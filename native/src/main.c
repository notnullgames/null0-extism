#include <extism.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "zip.h"

// this represents all shared things for a cart
typedef struct {
  struct zip_t *zip;
} Null0Cart;

// pull a string from an input param, in a host-function
const char *get_string_from_cart_input(ExtismCurrentPlugin *plugin,
                                       const ExtismVal input, size_t *len) {
  uint64_t offs = input.v.i64;
  *len = extism_current_plugin_memory_length(plugin, offs);
  return (const char *)extism_current_plugin_memory(plugin) + offs;
}

void null0_host_trace(ExtismCurrentPlugin *plugin, const ExtismVal *inputs,
                      ExtismSize ninputs, ExtismVal *outputs,
                      ExtismSize noutputs, void *userdata) {
  size_t len = 0;
  const char *message = get_string_from_cart_input(plugin, inputs[0], &len);
  printf("%s\n", message);
}

uint8_t *null0_file_read(Null0Cart cart, char *filename, size_t *bufsize) {
  void *buf = NULL;
  zip_entry_open(cart.zip, filename);
  zip_entry_read(cart.zip, &buf, bufsize);
  zip_entry_close(cart.zip);
  return (uint8_t *)buf;
}

// call when loading the car to set thigns up
Null0Cart null0_cart_load(char *filename) {
  Null0Cart cart;
  cart.zip = zip_open(filename, 0, 'r');
  return cart;
}

// call when unloading the cart to clean up
void null0_cart_unload(Null0Cart cart) { zip_close(cart.zip); }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: null0 <CART>\n");
    return 1;
  }

  // TODO: check magic-bytes for zip/wasm

  Null0Cart cart = null0_cart_load(argv[1]);

  ExtismValType trace_inputs[] = {ExtismValType_I32};
  ExtismValType trace_outputs[] = {};
  ExtismFunction *hf_trace =
      extism_function_new("trace", trace_inputs, 1, trace_outputs, 0,
                          null0_host_trace, (void *)&cart, NULL);
  extism_function_set_namespace(hf_trace, "null0");

  const ExtismFunction *functions[] = {hf_trace};

  size_t bufsize;
  uint8_t *wasmBytes = null0_file_read(cart, "main.wasm", &bufsize);
  char *errmsg = NULL;
  ExtismPlugin *plugin =
      extism_plugin_new(wasmBytes, bufsize, functions, 1, true, &errmsg);
  free(wasmBytes);

  extism_plugin_call(plugin, "_start", NULL, 0);

  if (plugin == NULL) {
    fprintf(stderr, "ERROR: %s\n", errmsg);
    extism_plugin_new_error_free(errmsg);
    extism_function_free(hf_trace);
    return 1;
  }

  extism_plugin_free(plugin);
  extism_function_free(hf_trace);

  null0_cart_unload(cart);
  return 0;
}
