//Corresponding header
#include "Engine.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <string>
#include <thread>

//Other libraries headers

//Own components headers
#include "managers/DrawMgr.h"
#include "managers/TimerMgr.h"
#include "utils/Log.h"

int32_t Engine::init() {
  //initialise monitor to FullHD
  constexpr int32_t MONITOR_WIDTH = 1920;
  constexpr int32_t MONITOR_HEIGHT = 1080;
  // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS
  constexpr int32_t DISPLAY_MODE = 1 | 16;

  const ManagerHandlerCfg cfg(DISPLAY_MODE, MONITOR_WIDTH, MONITOR_HEIGHT);

  if (EXIT_SUCCESS != _managerHandler.init(cfg)) {
    LOGERR("Error in _managerHandler.init()");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _game.init()) {
    LOGERR("Error, _game.init() failed");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _debugConsole.init()) {
    LOGERR("Error in _debugConsole.init()");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _inputEvent.init()) {
    LOGERR("Error in _inputEvent.init()");
    return EXIT_FAILURE;
  }

  constexpr auto maxFrameRate = 60;
  _debugConsole.setMaxFrameRate(maxFrameRate);
  gDrawMgr->setMaxFrameRate(maxFrameRate);
  gTimerMgr->onInitEnd();

  return EXIT_SUCCESS;
}

void Engine::deinit() {
  _game.deinit();
  _managerHandler.deinit();
  _inputEvent.deinit();
}

void Engine::mainLoop() {
  Time fpsTime;
  uint32_t elapsedTime = 0;

  while (true) {
    //begin measure the new frame elapsed time
    fpsTime.getElapsed();

    if (processFrame()) {
      break;
    }

    elapsedTime = static_cast<uint32_t>(fpsTime.getElapsed().toMicroseconds());

    if (_debugConsole.isActive()) {
      _debugConsole.update(elapsedTime);
    }

    limitFPS(elapsedTime);
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

void Engine::limitFPS(uint32_t elapspedTime) {
  const uint32_t MAX_MICROSECONDS_PER_FRAME = MICROSECOND
      / gDrawMgr->getMaxFrameRate();

  const uint32_t delay = ((MAX_MICROSECONDS_PER_FRAME < elapspedTime) ?
      0 : MAX_MICROSECONDS_PER_FRAME - elapspedTime);

  //Sleep the logic thread if max FPS is reached.
  //No need to struggle the CPU.
  std::this_thread::sleep_for(std::chrono::microseconds(delay));
}

