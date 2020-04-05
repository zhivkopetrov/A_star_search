#ifndef GAME_GAMEENTITIES_H_
#define GAME_GAMEENTITIES_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "GridContainer.h"
#include "ObstacleHandler.h"
#include "GameCursor.h"
#include "optionsmenu/OptionSelector.h"
#include "proxies/GameProxyInterface.hpp"
#include "pathfinding/PathGenerator.h"
#include "animators/AnimatorHandler.h"

//Forward declarations
class InputEvent;

class Game: private GameProxyInterface {
public:
  Game() = default;
  virtual ~Game() = default;

  int32_t init();

  void deinit();

  void draw();

  void handleEvent(const InputEvent &e);

private:
  void evaluateAStar();

  virtual void onNodeChanged(const NodeType nodeType, const Point &nodePosY)
      override final;

  virtual void onAnimFinished(const AnimType animType) override final;

  virtual void onOptionChanged(const Option option, const std::any &value)
      override final;

  virtual void onOptionAnimStatusChange(const OptionAnimStatus type)
      override final;

  PathGenerator _pathGenerator;

  GridContainer _gridContainer;

  ObstacleHandler _obstacleHandler;

  AnimatorHandler _animHandler;

  OptionSelector _optionSelector;

  GameCursor _gameCursor;
};

#endif /* GAME_GAMEENTITIES_H_ */

