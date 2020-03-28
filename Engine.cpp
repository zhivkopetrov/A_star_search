//Corresponding header
#include "Engine.h"

//C system headers
#include <unistd.h>

//C++ system headers
#include <cstdlib>
#include <string>

//Other libraries headers

//Own components headers
#include "managers/DrawMgr.h"
#include "managers/TimerMgr.h"
#include "utils/Log.h"

int32_t Engine::init() {
  int32_t err = EXIT_SUCCESS;

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
    if (EXIT_SUCCESS != _game.init()) {
      LOGERR("Error, _game.init() failed");
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
    if (EXIT_SUCCESS != _inputEvent.init()) {
      LOGERR("Error in _inputEvent.init()");
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

void Engine::deinit() {
  _game.deinit();
  _managerHandler.deinit();
  _inputEvent.deinit();
}

void Engine::mainLoop() {
  Time fpsTime;
  uint32_t fpsDelay = 0;

  while (true) {
    //begin measure the new frame elapsed time
    fpsTime.getElapsed();

    if (processFrame()) {
      break;
    }

    fpsDelay = static_cast<uint32_t>(fpsTime.getElapsed().toMicroseconds());

    if (_debugConsole.isActive()) {
      _debugConsole.update(fpsDelay);
    }

    const uint32_t MAX_MICROSECONDS_PER_FRAME = MICROSECOND
        / gDrawMgr->getMaxFrameRate();
    MAX_MICROSECONDS_PER_FRAME < fpsDelay ?
        fpsDelay = 0 : fpsDelay = MAX_MICROSECONDS_PER_FRAME - fpsDelay;

    //Sleep the logic thread if max FPS is reached.
    //No need to struggle the CPU.
    usleep(fpsDelay);
  }
}

bool Engine::processFrame() {
  _managerHandler.process();

  while (_inputEvent.pollEvent()) {
    if (_inputEvent.checkForExitRequest()) {
      return true;
    }

    handleEvent();
  }

  drawFrame();

  return false;
}

void Engine::drawFrame() {
  gDrawMgr->clearScreen();

  _game.draw();

  if (_debugConsole.isActive()) {
    _debugConsole.draw();
  }

  gDrawMgr->finishFrame();
}

void Engine::handleEvent() {
  _debugConsole.handleEvent(_inputEvent);
  _game.handleEvent(_inputEvent);
}

