#define EXTISM_IMPLEMENTATION
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extism-pdk.h"

typedef enum GamepadButton {
  GAMEPAD_BUTTON_UNKNOWN = 0,     // Unknown button, just for error checking
  GAMEPAD_BUTTON_UP = 1,          // Gamepad left DPAD up button
  GAMEPAD_BUTTON_RIGHT,           // Gamepad left DPAD right button
  GAMEPAD_BUTTON_DOWN,            // Gamepad left DPAD down button
  GAMEPAD_BUTTON_LEFT,            // Gamepad left DPAD left button
  GAMEPAD_BUTTON_Y,               // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
  GAMEPAD_BUTTON_B,               // Gamepad right button right (i.e. PS3: Square, Xbox: X)
  GAMEPAD_BUTTON_A,               // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
  GAMEPAD_BUTTON_X,               // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
  GAMEPAD_BUTTON_LEFT_SHOULDER,   // Gamepad top/back trigger left (first), it could be a trailing button
  GAMEPAD_BUTTON_LEFT_TRIGGER,    // Gamepad top/back trigger left (second), it could be a trailing button
  GAMEPAD_BUTTON_RIGHT_SHOULDER,  // Gamepad top/back trigger right (one), it could be a trailing button
  GAMEPAD_BUTTON_RIGHT_TRIGGER,   // Gamepad top/back trigger right (second), it could be a trailing button
  GAMEPAD_BUTTON_SELECT,          // Gamepad center buttons, left one (i.e. PS3: Select)
  GAMEPAD_BUTTON_MENU,            // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
  GAMEPAD_BUTTON_START,           // Gamepad center buttons, right one (i.e. PS3: Start)
  GAMEPAD_BUTTON_LEFT_THUMB,      // Gamepad joystick pressed button left
  GAMEPAD_BUTTON_RIGHT_THUMB,     // Gamepad joystick pressed button right
} GamepadButton;

// host functions

EXTISM_IMPORT("extism:host/user", "trace")
extern void _null0_trace_real(ExtismHandle);

// max-size for trace messages
#ifndef NULL0_TRACE_SIZE
#define NULL0_TRACE_SIZE 1024 * 64
#endif

// Log a string (using printf-style)
void trace(const char* format, ...) {
  char null0_traceBuffer[NULL0_TRACE_SIZE] = {};
  va_list args;
  va_start(args, format);
  vsnprintf(null0_traceBuffer, NULL0_TRACE_SIZE, format, args);
  va_end(args);

  int l = strlen(null0_traceBuffer);
  ExtismHandle handle = extism_alloc(l);
  extism_store_to_handle(handle, 0, null0_traceBuffer, l);
  _null0_trace_real(handle);
}

// USER CALLBACKS

#pragma weak load
void load();
int32_t EXTISM_EXPORTED_FUNCTION(_extism_load) {
  if (load) {
    load();
  }
  return 0;
}

#pragma weak unload
void unload();
int32_t EXTISM_EXPORTED_FUNCTION(_extism_unload) {
  if (unload) {
    unload();
  }
  return 0;
}

#pragma weak update
void update();
int32_t EXTISM_EXPORTED_FUNCTION(_extism_update) {
  if (update) {
    update();
  }
  return 0;
}

#pragma weak buttonDown
void buttonDown(GamepadButton button);
int32_t EXTISM_EXPORTED_FUNCTION(_extism_buttonDown) {
  if (buttonDown) {
    size_t l = sizeof(GamepadButton);
    int inputData[l];
    extism_load_input(0, inputData, l);
    buttonDown((GamepadButton)inputData);
  }
  return 0;
}

#pragma weak buttonUp
void buttonUp(GamepadButton button);
int32_t EXTISM_EXPORTED_FUNCTION(_extism_buttonUp) {
  if (buttonUp) {
    size_t l = sizeof(GamepadButton);
    int inputData[l];
    extism_load_input(0, inputData, l);
    buttonUp((GamepadButton)inputData);
  }
  return 0;
}
