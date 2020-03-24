#ifndef UTILS_DRAWING_DEBUGCONSOLE_H_
#define UTILS_DRAWING_DEBUGCONSOLE_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/drawing/Text.h"

//Forward declarations
union SDL_Event;

class DebugConsole {
public:
  DebugConsole();
  virtual ~DebugConsole() = default;

  int32_t init();

  /** @ brief used to toggle active/inactive status of the debug console
   * */
  void handleEvent(const SDL_Event &e);

  void update(const uint32_t elapsedTime);

  void draw();

  inline bool isActive() const {
    return _isActive;
  }

  inline void setMaxFrameRate(const uint32_t maxFrames) {
    _maxFrames = maxFrames;
  }

private:
  Text _fpsText;

  uint32_t _maxFrames;

  int32_t _updateCounter;

  bool _isActive;
};

#endif /* UTILS_DRAWING_DEBUGCONSOLE_H_ */

