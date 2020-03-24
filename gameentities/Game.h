#ifndef GAME_GAMEENTITIES_H_
#define GAME_GAMEENTITIES_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "GridContainer.h"
#include "proxies/GameProxyInterface.hpp"
#include "animators/AnimatorHandler.h"
#include "pathfinding/MazeGenerator.h"

//Forward declarations

class Game: private GameProxyInterface {
public:
  Game() = default;
  virtual ~Game() = default;

  int32_t init(const bool isDiagonalMovementAllowed);

  void deinit();

  void draw();

  void handleUserEvent(const SDL_Event &e);

private:
  void evaluateAStar();

  virtual void onNodeChanged(const NodeType nodeType, const Point &nodePosY)
      override final;

  virtual void onAnimFinished() override final;

  MazeGenerator _generator;

  GridContainer _gridContainer;

  AnimatorHandler _animHandler;
};

#endif /* GAME_GAMEENTITIES_H_ */

