#ifndef ENGINE_H_
#define ENGINE_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "managers/ManagerHandler.h"
#include "gameentities/Game.h"
#include "sdl/InputEvent.h"
#include "utils/drawing/DebugConsole.h"

//Forward declarations

class Engine {
public:
  Engine() = default;
  ~Engine() = default;

  int32_t init();

  void deinit();

  inline void start() {
    mainLoop();
  }

private:
  void mainLoop();

  bool processFrame();

  void drawFrame();

  void handleEvent();

  void limitFPS(uint32_t elapspedTime);

  ManagerHandler _managerHandler;

  Game _game;

  InputEvent _inputEvent;

  DebugConsole _debugConsole;
};

#endif /* ENGINE_H_ */
