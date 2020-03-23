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
          Textures::A_STAR_PATH, Textures::WALL)) {
    LOGERR("Error in _gridLineContainer.init()");

    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    HeuristicFunction heuristic = Heuristic::manhattan;

    //diagonal heuristic is better for diagonal movements
    if (isDiagonalMovementAllowed) {
      heuristic = Heuristic::diagonal;
    }

    if (EXIT_SUCCESS != _generator.init(Grid::GRID_WIDTH, Grid::GRID_HEIGHT,
            isDiagonalMovementAllowed, heuristic)) {
      LOGERR("Error, _generator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _generalTextAnimator.init()) {
      LOGERR("Error, _generalTextAnimator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _pathAnimator.init(&_gridContainer,
            Textures::BATMAN_SMALL, Timers::PATH_TIMER_ID)) {
      LOGERR("Error, _pathAnimator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _scaleAnimator.init(&_pathAnimator,
            Textures::BATMAN_BIG, Timers::SCALE_TIMER_ID)) {
      LOGERR("Error, _pathAnimator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  return err;
}

void Game::deinit() {

}

void Game::draw() {
  _gridContainer.draw();
  _generalTextAnimator.draw();

  if (_pathAnimator.isActive()) {
    _pathAnimator.draw();
  }

  if (_scaleAnimator.isActive()) {
    _scaleAnimator.draw();
  }
}

void Game::handleUserEvent(SDL_Event &e) {
  _gridContainer.handleUserEvent(e);

  if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE) {
    if (_generator.isReadyToEvaluate() && !_scaleAnimator.isActive()) {
      evaluateAStar();
    }
  }
}

void Game::onNodeChanged(const NodeType nodeType, const int32_t nodeX,
                         const int32_t nodeY) {
  switch (nodeType) {
  case NodeType::WALL_ADD:
    _generator.addCollision( { nodeX, nodeY });

    _gridContainer.clearGridFromAStarPathNodes();
    _gridContainer.addCollision(nodeX, nodeY);
    break;

  case NodeType::WALL_REMOVE:
    _generator.removeCollision( { nodeX, nodeY });

    _gridContainer.clearGridFromAStarPathNodes();
    _gridContainer.removeCollision(nodeX, nodeY);
    break;

  case NodeType::START_CHANGE:
    _scaleAnimator.setTargetPos(
        _gridContainer.getNodeCoordinates(nodeX, nodeY));
    _generator.setStartNodePos( { nodeX, nodeY });

    _gridContainer.clearGridFromAStarPathNodes();
    _gridContainer.addStartNode(nodeX, nodeY);
    break;

  case NodeType::END_CHANGE:
    _generator.setEndNodePos( { nodeX, nodeY });

    _gridContainer.clearGridFromAStarPathNodes();
    _gridContainer.addEndNode(nodeX, nodeY);
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
  std::vector<Point> path = _generator.findPath();

  if ( (path.front() != _generator.getEndNodePos())) {
    _generalTextAnimator.showNoPathText();
  } else {
    _generalTextAnimator.hideNoPathText();

    _pathAnimator.loadPath(path);
    _scaleAnimator.startAnim();
  }
}

