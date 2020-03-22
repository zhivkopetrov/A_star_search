//Corresponding header
#include "DrawMgr.h"

#include <cstdlib>

//Other libraries headers
#include "sdl/Renderer.h"
#include "sdl/MonitorWindow.h"

//Own components headers
#include "utils/Log.h"

DrawMgr *gDrawMgr = nullptr;

DrawMgr::DrawMgr(const int32_t displayMode, const int32_t monitorWidth,
                 const int32_t monitorHeight)
    : _renderer(nullptr), _window(nullptr), _MONITOR_WIDTH(monitorWidth),
      _MONITOR_HEIGHT(monitorHeight), _displayMode(displayMode), _maxFrames(0) {
}

int32_t DrawMgr::init() {
  int32_t err = EXIT_SUCCESS;

  _window = new MonitorWindow(_MONITOR_WIDTH, _MONITOR_HEIGHT);

  if (nullptr == _window) {
    LOGERR("Error, bad alloc for MonitorWindow");
    err = EXIT_FAILURE;
  } else {
    if (EXIT_SUCCESS != _window->init(_displayMode)) {
      LOGERR("Error, _window->init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (err == EXIT_SUCCESS) {
    _renderer = new Renderer(_window->getWindow());

    if (nullptr == _renderer) {
      LOGERR("Error, bad alloc for Renderer");
      err = EXIT_FAILURE;
    } else {
      if (EXIT_SUCCESS != _renderer->init()) {
        LOGERR("Error, _renderer->init() failed");
        err = EXIT_FAILURE;
      }
    }
  }

  return err;
}

void DrawMgr::deinit() {
  if (_renderer) {
    _renderer->deinit();

    delete _renderer;
    _renderer = nullptr;
  }

  if (_window) {
    _window->deinit();

    delete _window;
    _window = nullptr;
  }
}

const char* DrawMgr::getName() {
  return "DrawMgr";
}

void DrawMgr::process() {

}

void DrawMgr::clearScreen() {
  _renderer->clearScreen();
}

void DrawMgr::finishFrame() {
  _renderer->finishFrame();
}

void DrawMgr::addDrawCmd(DrawParams *drawParams) {
  _renderer->drawTexture(drawParams);
}

void DrawMgr::addDrawCmdArr(DrawParams drawParamsArr[], const int32_t size) {
  _renderer->drawTextureArr(drawParamsArr, size);
}
