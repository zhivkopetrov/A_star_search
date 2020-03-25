//Corresponding header
#include "Game.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "common/CommonDefines.h"
#include "utils/Log.h"

int32_t Game::init(const bool isDiagonalMovementAllowed) {
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
    HeuristicFunction heuristic = Heuristic::manhattan;

    //diagonal heuristic is better for diagonal movements
    if (isDiagonalMovementAllowed) {
      heuristic = Heuristic::diagonal;
    }

    if (EXIT_SUCCESS != _pathGenerator.init(Grid::GRID_WIDTH, Grid::GRID_HEIGHT,
            isDiagonalMovementAllowed, heuristic, &_obstacleHandler)) {
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

  return err;
}

void Game::deinit() {

}

void Game::draw() {
  _gridContainer.draw();
  _animHandler.draw();
}

void Game::handleUserEvent(const SDL_Event &e) {
  if (_animHandler.isActive()) {
    return;
  }

  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {

    case SDLK_SPACE:
      if (_gridContainer.isReadyToEvaluate()) {
        evaluateAStar();
      }
      break;

    case SDLK_c:
      _gridContainer.clearGrid();
      break;
    }
  }

  _gridContainer.handleUserEvent(e);
  _obstacleHandler.handleUserEvent(e);
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

