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

uint8_t* null0_file_read(Null0Cart cart, char* filename, size_t* bufsize) {
  void *buf = NULL;
  zip_entry_open(cart.zip, filename);
  zip_entry_read(cart.zip, &buf, bufsize);
  zip_entry_close(cart.zip);
  return (uint8_t*)buf;
}

// call when loading the car to set thigns up
Null0Cart null0_cart_load(char* filename) {
  Null0Cart cart;
  cart.zip = zip_open(filename, 0, 'r');
  return cart;
}

// call when unloading the cart to clean up
void null0_cart_unload(Null0Cart cart) {
  zip_close(cart.zip);
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: null0 <CART>\n");
    return 1;
  }
  
  Null0Cart cart = null0_cart_load(argv[1]);
  
  size_t bufsize;
  uint8_t* wasmBytes = null0_file_read(cart, "main.wasm", &bufsize);


  free(wasmBytes);

  null0_cart_unload(cart);
  return 0;
}