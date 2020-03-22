//Corresponding header
#include "Game.h"

//C system headers
#include <unistd.h>

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "managers/DrawMgr.h"
#include "managers/TimerMgr.h"
#include "utils/time/Time.h"
#include "utils/Log.h"

Game::Game() {

}

int32_t Game::init(const bool isDiagonalMovementAllowed) {
  int32_t err = EXIT_SUCCESS;
  memset(&_inputEvent, 0, sizeof (_inputEvent));

  //initialise monitor to FullHD
  constexpr int32_t MONITOR_WIDTH = 1920;
  constexpr int32_t MONITOR_HEIGHT = 1080;
  // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS
  constexpr int32_t DISPLAY_MODE = 1 | 16;

  const ManagerHandlerCfg cfg(DISPLAY_MODE, MONITOR_WIDTH, MONITOR_HEIGHT);

  if (EXIT_SUCCESS != _managerHandler.init(cfg)) {
    LOGERR("Error in _managerHandler.init()");
    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _gridContainer.init(this, Textures::VERTICAL_LINE,
            Textures::HORIZONTAL_LINE, Textures::START_NODE, Textures::END_NODE,
            Textures::A_STAR_PATH, Textures::WALL)) {
      LOGERR("Error in _gridLineContainer.init()");

      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _debugConsole.init()) {
      LOGERR("Error in _debugConsole.init()");
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

  if (EXIT_SUCCESS == err) {
    constexpr auto maxFrameRate = 60;
    _debugConsole.setMaxFrameRate(maxFrameRate);
    gDrawMgr->setMaxFrameRate(maxFrameRate);
    gTimerMgr->onInitEnd();
  }

  return err;
}

void Game::deinit() {
  _managerHandler.deinit();
}

void Game::mainLoop() {
  Time fpsTime;
  uint32_t fpsDelay = 0;

  while (true) {
    //begin measure the new frame elapsed time
    fpsTime.getElapsed();

    if (handleUserEvent(_inputEvent)) {
      break;
    }

    updateWorldState();
    drawWorld();

    fpsDelay = static_cast<uint32_t>(fpsTime.getElapsed().toMicroseconds());

    if(_debugConsole.isActive()) {
      _debugConsole.update(fpsDelay);
    }

    const uint32_t MAX_MICROSECONDS_PER_FRAME =
                           MICROSECOND / gDrawMgr->getMaxFrameRate();
    MAX_MICROSECONDS_PER_FRAME < fpsDelay ?
            fpsDelay = 0 :
            fpsDelay = MAX_MICROSECONDS_PER_FRAME - fpsDelay;

    //Sleep the logic thread if max FPS is reached.
    //No need to struggle the CPU.
    usleep(fpsDelay);
  }
}

void Game::updateWorldState() {
  _managerHandler.process();
}

void Game::drawWorld() {
  gDrawMgr->clearScreen();

  _gridContainer.draw();

  _generalTextAnimator.draw();

  if (_debugConsole.isActive()) {
    _debugConsole.draw();
  }

  gDrawMgr->finishFrame();
}

bool Game::handleUserEvent(SDL_Event &e) {
  while (0 != SDL_PollEvent(&e)) {
    if ( (SDLK_ESCAPE == e.key.keysym.sym && e.type == SDL_KEYUP) || (SDL_QUIT
        == e.type)) {
      return true;
    }

    _debugConsole.handleEvent(e);
    _gridContainer.handleUserEvent(e);
  }

  return false;
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

