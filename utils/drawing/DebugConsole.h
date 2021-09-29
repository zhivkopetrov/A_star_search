#ifndef UTILS_DRAWING_DEBUGCONSOLE_H_
#define UTILS_DRAWING_DEBUGCONSOLE_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/drawing/Text.h"

//Forward declarations
class InputEvent;

class DebugConsole {
public:
  DebugConsole();

  int32_t init();

  /** @ brief used to toggle active/inactive status of the debug console
   * */
  void handleEvent(const InputEvent &e);

  void update(const int64_t elapsedTime);

  void draw();

  inline bool isActive() const {
    return _isActive;
  }

  inline void setMaxFrameRate(const uint32_t maxFrames) {
    _maxFrames = maxFrames;
  }

private:
  Text _fpsText;

  int64_t _maxFrames;

  int32_t _updateCounter;

  bool _isActive;
};

#endif /* UTILS_DRAWING_DEBUGCONSOLE_H_ */

