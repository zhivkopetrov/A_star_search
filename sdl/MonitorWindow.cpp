//Corresponding header
#include "MonitorWindow.h"

//C system headers

//C++ system headers
#include <string>

//Other libraries headers
#include <SDL_video.h>

//Own components headers
#include "utils/Log.h"

//Window modes:
//SDL_WINDOW_SHOWN - for windowed version
//SDL_WINDOW_FULLSCREEN_DESKTOP - for fullscreen
//SDL_WINDOW_OPENGL for working with OPEN_GL windows and context

MonitorWindow::MonitorWindow(const int32_t windowWidth,
                             const int32_t windowHeight, const Point &windowPos)
    : _window(nullptr),
      _windowRect(windowPos.x, windowPos.y, windowWidth, windowHeight) {

}

MonitorWindow::~MonitorWindow() {
  deinit();
}

int32_t MonitorWindow::init(const int32_t displayMode) {
  int32_t err = EXIT_SUCCESS;

  int32_t initWindowX = 0;
  int32_t initWindowY = 0;

  const Point POS(_windowRect.x, _windowRect.y);

  if (Point::UNDEFINED == POS) {
    initWindowX = SDL_WINDOWPOS_UNDEFINED;
    initWindowY = SDL_WINDOWPOS_UNDEFINED;
  } else {
    initWindowX = _windowRect.x;
    initWindowY = _windowRect.y;
  }

  //Create window
  _window = SDL_CreateWindow("Batman", initWindowX, initWindowY,
      _windowRect.w, _windowRect.h, displayMode);

  if (nullptr == _window) {
    LOGERR("Window could not be created! SDL Error: %s", SDL_GetError());

    err = EXIT_FAILURE;
  } else {
    //obtain real window coordinates after creation
    SDL_GetWindowPosition(_window, &_windowRect.x, &_windowRect.y);
  }

  return err;
}

void MonitorWindow::deinit() {
  if (_window) //sanity check
  {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
}

