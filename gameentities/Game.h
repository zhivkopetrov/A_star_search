#ifndef GAME_GAMEENTITIES_H_
#define GAME_GAMEENTITIES_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "GameProxyInterface.hpp"
#include "GridContainer.h"
#include "animators/PathAnimator.h"
#include "animators/GeneralTextAnimator.h"
#include "pathfinding/MazeGenerator.h"

//Forward declarations

class Game : private GameProxyInterface {
public:
  Game() = default;
  virtual ~Game() = default;

  int32_t init(const bool isDiagonalMovementAllowed);

  void deinit();

  void draw();

  void handleUserEvent(SDL_Event &e);

private:
  void evaluateAStar();

  virtual void onNodeChanged(const NodeType nodeType, const int32_t nodeX,
                             const int32_t nodeY) override final;

  MazeGenerator _generator;

  GridContainer _gridContainer;

  PathAnimator _pathAnimator;

  GeneralTextAnimator _generalTextAnimator;
};

#endif /* GAME_GAMEENTITIES_H_ */

