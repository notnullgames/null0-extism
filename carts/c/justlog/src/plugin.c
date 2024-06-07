#define EXTISM_IMPLEMENTATION
#include "../extism/extism-pdk.h"

int32_t hello() {
  extism_output_buf_from_sz("Hello, world!");
  return 0;
}
