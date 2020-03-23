//Corresponding header
#include "Game.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "utils/Log.h"

int32_t Game::init(const bool isDiagonalMovementAllowed) {
  int32_t err = EXIT_SUCCESS;

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _gridContainer.init(this, Textures::VERTICAL_LINE,
            Textures::HORIZONTAL_LINE, Textures::START_NODE, Textures::END_NODE,
            Textures::A_STAR_PATH, Textures::WALL)) {
      LOGERR("Error in _gridLineContainer.init()");

      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    HeuristicFunction heuristic = Heuristic::manhattan;

    //diagonal heuristic is better for diagonal movements
    if (isDiagonalMovementAllowed) {
      heuristic = Heuristic::diagonal;
    }

    if (EXIT_SUCCESS != _generator.init(GRID_WIDTH,
    GRID_HEIGHT, isDiagonalMovementAllowed, heuristic)) {
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
    if (EXIT_SUCCESS !=
        _pathAnimator.init(&_gridContainer, Timers::PATH_TIMER_ID)) {
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
}

void Game::handleUserEvent(SDL_Event &e) {
  _gridContainer.handleUserEvent(e);
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
    break;
  }

  evaluateAStar();
}

void Game::evaluateAStar() {
  // This method returns vector of coordinates from target to source.
  std::vector<Point> path = _generator.findPath();

  if (path.empty()) {
    return;
  }

  if ( (path.front() != _generator.getEndNodePos())) {
    _generalTextAnimator.showNoPathText();
  } else {
    _generalTextAnimator.hideNoPathText();

    _pathAnimator.startAnimation(path);
  }
}

