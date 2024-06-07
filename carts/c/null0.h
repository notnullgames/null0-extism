#pragma once

#define EXTISM_IMPLEMENTATION
#include "extism-pdk.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NULL0_EXPORT(n) __attribute__((export_name(n)))
#define NULL0_IMPORT(n) __attribute__((import_module("null0"), import_name(n)))

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;

// max-size for trace messages
#ifndef NULL0_TRACE_SIZE
#define NULL0_TRACE_SIZE 1024 * 1024
#endif
char null0_traceBuffer[NULL0_TRACE_SIZE];

NULL0_IMPORT("trace")
void _null0_trace_real(char *str);

// Log a string (using printf-style)
void trace(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vsnprintf(null0_traceBuffer, NULL0_TRACE_SIZE, format, args);
  va_end(args);
  _null0_trace_real(null0_traceBuffer);
}

// user entry-point
void load();
NULL0_EXPORT("load")
int32_t _null0_load() {
  load();
  return 0;
}
