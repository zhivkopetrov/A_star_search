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
  _window = new MonitorWindow(_MONITOR_WIDTH, _MONITOR_HEIGHT);
  if (nullptr == _window) {
    LOGERR("Error, bad alloc for MonitorWindow");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _window->init(_displayMode)) {
    LOGERR("Error, _window->init() failed");
    return EXIT_FAILURE;
  }

  _renderer = new Renderer(_window->getWindow());
  if (nullptr == _renderer) {
    LOGERR("Error, bad alloc for Renderer");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != _renderer->init()) {
    LOGERR("Error, _renderer->init() failed");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
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

void DrawMgr::addDrawCmd(const DrawParams *drawParams) {
  _renderer->drawTexture(drawParams);
}

void DrawMgr::addDrawCmdArr(const DrawParams drawParamsArr[], const size_t size) {
  _renderer->drawTextureArr(drawParamsArr, size);
}

int32_t DrawMgr::lockRenderer() {
  return _renderer->lockRenderer();
}

int32_t DrawMgr::unlockRenderer() {
  return _renderer->unlockRenderer();
}

void DrawMgr::changeRendererTarget(const int32_t FBOId) {
  _renderer->changeRendererTarget(FBOId);
}

void DrawMgr::resetRendererTarget() {
  _renderer->resetRendererTarget();
}

void DrawMgr::clearCurrentRendererTarget(const Color &clearColor) {
  _renderer->clearCurrentRendererTarget(clearColor);
}

void DrawMgr::updateCurrRendererTarget(const DrawParams drawParamsArr[],
                                       const size_t size) {
  _renderer->updateCurrRendererTarget(drawParamsArr, size);
}

void DrawMgr::setWidgetBlendMode(const DrawParams &widgetInfo,
                                 const BlendMode blendMode) {
  _renderer->setWidgetBlendMode(widgetInfo, blendMode);
}

void DrawMgr::setWidgetOpacity(const DrawParams &widgetInfo,
                               const uint8_t opacity) {
  _renderer->setWidgetOpacity(widgetInfo, opacity);
}


