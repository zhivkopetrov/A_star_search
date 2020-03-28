//Corresponding header
#include "Game.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "sdl/InputEvent.h"
#include "common/CommonDefines.h"
#include "utils/Log.h"

int32_t Game::init() {
  int32_t err = EXIT_SUCCESS;

  if (EXIT_SUCCESS != _gridContainer.init(this, Textures::VERTICAL_LINE,
          Textures::HORIZONTAL_LINE, Textures::START_NODE, Textures::END_NODE,
          Textures::A_STAR_PATH, Textures::WALL, Textures::OBSTACLES)) {
    LOGERR("Error in _gridLineContainer.init()");

    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _obstacleHandler.init(&_gridContainer, "../levels/",
            Grid::OBSTACLE_LEVELS)) {
      LOGERR("Error, _obstacleHandler.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _pathGenerator.init(Grid::GRID_WIDTH, Grid::GRID_HEIGHT,
            &_obstacleHandler)) {
      LOGERR("Error, _pathGenerator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _animHandler.init(this, &_gridContainer)) {
      LOGERR("Error, _animHandler.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _optionSelector.init(this)) {
      LOGERR("Error, _optionSelector.init() failed");
      err = EXIT_FAILURE;
    }
  }

  return err;
}

void Game::deinit() {

}

void Game::draw() {
  _gridContainer.draw();
  _animHandler.draw();
  _optionSelector.draw();
}

void Game::handleEvent(const InputEvent &e) {
  if (_animHandler.isActive()) {
    return;
  }

  if (TouchEvent::KEYBOARD_RELEASE == e.type) {
    switch (e.key) {

    case Keyboard::KEY_SPACE:
      if (_gridContainer.isReadyToEvaluate()) {
        evaluateAStar();
      }
      break;

    case Keyboard::KEY_C:
      _gridContainer.clearGrid();
      break;

      //TODO temporary - remove after buttons are implemented in the OptionSelector
    case Keyboard::KEY_D:
      _optionSelector.setOption(Option::DIAGONAL_MOVEMENT, true);
      break;

      //TODO temporary - remove after buttons are implemented in the OptionSelector
    case Keyboard::KEY_UP:
      _obstacleHandler.loadNextLevel();
      break;

      //TODO temporary - remove after buttons are implemented in the OptionSelector
    case Keyboard::KEY_DOWN:
      _obstacleHandler.loadPreviousLevel();
      break;
    }
  }

  _gridContainer.handleEvent(e);
  _optionSelector.handleEvent(e);
}

void Game::onNodeChanged(const NodeType nodeType, const Point &nodePos) {
  switch (nodeType) {
  case NodeType::WALL_ADD:
    _obstacleHandler.addObstacle(nodePos);
    break;

  case NodeType::NODE_REMOVE:
    _obstacleHandler.removeObstacle(nodePos);
    break;

  case NodeType::START_CHANGE:
    _animHandler.perform(AnimEvent::SET_SCALE_AMIM_START_TARGET,
        _gridContainer.getNodeCoordinates(nodePos));
    break;

  case NodeType::END_CHANGE:
    _animHandler.perform(AnimEvent::SET_SCALE_AMIM_END_TARGET,
        _gridContainer.getNodeCoordinates(nodePos));
    break;

  default:
    LOGERR("Error, received unknown NodeType: %d",
        static_cast<int32_t>(nodeType))
    ;
    break;
  }
}

void Game::evaluateAStar() {
  // This method returns vector of coordinates from target to source.
  // the data will be moved, so it's not const
  const Point startNode = _gridContainer.getStartNodePos();
  const Point endNode = _gridContainer.getEndNodePos();
  std::vector<Point> path = _pathGenerator.findPath(startNode, endNode);

  if (path.front() != endNode) {
    _animHandler.perform(AnimEvent::START_NO_PATH_ANIM);
  } else {
    _animHandler.perform(AnimEvent::LOAD_ANIM_PATH, &path);
    _animHandler.perform(AnimEvent::START_PATH_ANIM);
  }
}

void Game::onEndAnimFinished() {
  _gridContainer.clearGrid();
}

void Game::onOptionChanged(const Option option, const std::any &value) {
  switch (option) {
    case Option::DIAGONAL_MOVEMENT:
      _pathGenerator.changeDiagonalOption(value);
      break;

    default:
      LOGERR("Error, received unknown Option: %d",
          static_cast<int32_t>(option))
      ;
      break;
  }
}

