//Corresponding header
#include "Game.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "sdl/InputEvent.h"
#include "common/CommonDefines.h"
#include "utils/EnumClassUtils.hpp"
#include "utils/Log.h"

int32_t Game::init() {
  if (EXIT_SUCCESS != _gridContainer.init(this, Textures::VERTICAL_LINE,
          Textures::HORIZONTAL_LINE, Textures::START_NODE, Textures::END_NODE,
          Textures::A_STAR_PATH, Textures::WALL, Textures::OBSTACLES)) {
    LOGERR("Error in _gridLineContainer.init()");

    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _obstacleHandler.init(&_gridContainer, "../levels/",
          Grid::OBSTACLE_LEVELS)) {
    LOGERR("Error, _obstacleHandler.init() failed");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _pathGenerator.init(Grid::GRID_WIDTH, Grid::GRID_HEIGHT,
          &_obstacleHandler)) {
    LOGERR("Error, _pathGenerator.init() failed");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _animHandler.init(this, &_gridContainer)) {
    LOGERR("Error, _animHandler.init() failed");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _optionSelector.init(this)) {
    LOGERR("Error, _optionSelector.init() failed");
    return EXIT_FAILURE;
  }

  constexpr int32_t cursorClickX = 29;
  constexpr int32_t cursorClickY = 24;
  if (EXIT_SUCCESS != _gameCursor.init("../assets/batman_gadget.png",
      cursorClickX, cursorClickY)) {
    LOGERR("Error, _gameCursor.init() failed");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void Game::deinit() {
  _gameCursor.deinit();
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
    }
  }

  if (_optionSelector.handleEvent(e)) {
    return;
  }
  _gridContainer.handleEvent(e);
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
    LOGERR("Error, received unknown NodeType: %hhu",
        getEnumClassValue(nodeType));
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

void Game::onAnimFinished(const AnimType animType) {
  if (AnimType::SPEECH_ANIM == animType) {
    _gridContainer.clearGrid();
  } else if (AnimType::MENU_OPEN_MOVE_ANIM == animType) {
    _optionSelector.onMoveAnimFinished(OptionAnimStatus::END_OPEN_ANIM);
  } else if (AnimType::MENU_CLOSE_MOVE_ANIM == animType) {
    _optionSelector.onMoveAnimFinished(OptionAnimStatus::END_CLOSE_ANIM);
  } else {
    LOGERR("Error, should not receive AnimType: %hhu here",
        getEnumClassValue(animType));
  }
}

void Game::onOptionChanged(const Option option, const std::any &value) {
  switch (option) {
  case Option::DIAGONAL_MOVEMENT:
    _pathGenerator.changeDiagonalOption(value);
    break;

  case Option::LEVEL_CHANGE:
    _obstacleHandler.changeLevel(value);
    _optionSelector.setTextLevelId(_obstacleHandler.getCurrLevelId());
    break;

  default:
    LOGERR("Error, received unknown Option: %hhu", getEnumClassValue(option));
    break;
  }
}

void Game::onOptionAnimStatusChange(const OptionAnimStatus type) {
  if (OptionAnimStatus::START_OPEN_ANIM == type) {
    _animHandler.perform(AnimEvent::START_OPEN_MENU_ANIM);
  } else if (OptionAnimStatus::START_CLOSE_ANIM == type) {
    _animHandler.perform(AnimEvent::START_CLOSE_MENU_ANIM);
  } else if (OptionAnimStatus::UPDATE_ANIM_CONTENT == type) {
    const auto widgetState = _optionSelector.getWidgetsState();
    _animHandler.perform(AnimEvent::UPDATE_MENU_ANIM_CONTENT, &widgetState);
  } else {
    LOGERR("Error, received unknown OptionAnimType: %hhu",
        getEnumClassValue(type));
  }
}

