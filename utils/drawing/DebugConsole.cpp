//Corresponding header
#include "DebugConsole.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <string>

//Other libraries headers

//Own components headers
#include "sdl/InputEvent.h"
#include "common/CommonDefines.h"
#include "utils/LimitValues.hpp"
#include "utils/time/Time.h"
#include "utils/Log.h"

#define UPDATE_SKIPS 20

DebugConsole::DebugConsole()
    : _maxFrames(INIT_UINT32_VALUE), _updateCounter(UPDATE_SKIPS),
      _isActive(false) {

}

int32_t DebugConsole::init() {
  _fpsText.create(Textures::FPS_TEXT, Point ( 20, 20 ), "0", FontSize::SMALL);

  return EXIT_SUCCESS;
}

void DebugConsole::handleEvent(const InputEvent &e) {
  if (TouchEvent::KEYBOARD_RELEASE == e.type) {
    if (Keyboard::KEY_TILDA == e.key) {
      _isActive = !_isActive;
    }
  }
}

//update fps text once in a while to get a stable(not constantly changing) image
void DebugConsole::update(const uint32_t elapsedTime) {
  --_updateCounter;
  if (0 < _updateCounter) {
    return;
  }
  _updateCounter = UPDATE_SKIPS;

  uint32_t frames = MICROSECOND / elapsedTime;
  if (frames > _maxFrames) {
    frames = _maxFrames;
  }

  _fpsText.setText(std::to_string(frames).c_str());
}

void DebugConsole::draw() {
  _fpsText.draw();
}

