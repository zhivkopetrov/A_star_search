#ifndef GAME_H_
#define GAME_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "GameProxyInterface.hpp"
#include "gameentities/GridContainer.h"
#include "gameentities/animators/PathAnimator.h"
#include "gameentities/animators/GeneralTextAnimator.h"
#include "pathfinding/MazeGenerator.h"
#include "managers/ManagerHandler.h"
#include "utils/DebugConsole.h"

//Forward declarations

class Game: GameProxyInterface {
public:
  Game();
  virtual ~Game() = default;

  int32_t init(const bool isDiagonalMovementAllowed);

  void deinit();

  inline void start() {
    mainLoop();
  }

private:
  void mainLoop();

  void updateWorldState();

  void drawWorld();

  bool handleUserEvent(SDL_Event &e);

  void evaluateAStar();

  virtual void onNodeChanged(const NodeType nodeType, const int32_t nodeX,
                             const int32_t nodeY) override final;

  ManagerHandler _managerHandler;

  MazeGenerator _generator;

  DebugConsole _debugConsole;

  GridContainer _gridContainer;

  PathAnimator _pathAnimator;

  GeneralTextAnimator _generalTextAnimator;

  SDL_Event _inputEvent;
};

#endif /* GAME_H_ */

