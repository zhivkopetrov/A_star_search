#ifndef ENGINE_H_
#define ENGINE_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "managers/ManagerHandler.h"
#include "gameentities/Game.h"
#include "utils/drawing/DebugConsole.h"

//Forward declarations

class Engine {
public:
  Engine() = default;
  ~Engine() = default;

  int32_t init(const bool isDiagonalMovementAllowed);

  void deinit();

  inline void start() {
    mainLoop();
  }

private:
  void mainLoop();

  void updateWorldState();

  void drawWorld();

  bool handleUserEvent(SDL_Event& e);

  ManagerHandler _managerHandler;

  Game _game;

  DebugConsole _debugConsole;
};

#endif /* ENGINE_H_ */
