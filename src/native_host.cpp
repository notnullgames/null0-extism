#define EXTISM_NO_JSON
#include "extism.hpp"

#include <cstring>
#include <fstream>
#include <iostream>

#include "zip_file.hpp"

using namespace extism;
using namespace std;
using namespace miniz_cpp;

#include <extism.hpp>

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

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Usage: null0 <CART>" << endl;
    return 1;
  }

  // TODO: check magic bytes on entry file (zip or wasm)
  // TODO: main.wasm exists
  // TODO: check magic bytes on main.wasm
  // TODO: pass shared cart object with filesystem, screen, vectors of assets, etc

  zip_file file(argv[1]);
  string bytesS = file.read("main.wasm");
  vector<uint8_t> bytes;
  bytes.assign(bytesS.begin(), bytesS.end());

  auto trace_in = std::vector<extism::ValType>{extism::ValType::ExtismValType_I64};
  auto null0_host_trace = extism::Function(
      "trace",
      trace_in,
      {},
      [](extism::CurrentPlugin plugin, void* user_data) {
        cout << plugin.inputStringView(0) << endl;
      },
      NULL,
      NULL);

  Plugin plugin(bytes, true, {null0_host_trace});
  plugin.call("_extism_load");

  // test user callback
  const vector<uint8_t> args = {GAMEPAD_BUTTON_START};
  plugin.call("_extism_buttonDown", args);

  return 0;
}
