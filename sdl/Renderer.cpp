//Corresponding header
#include "Renderer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_hints.h>

//Own components headers
#include "Texture.h"
#include "managers/RsrcMgr.h"
#include "utils/Log.h"

Renderer::Renderer(SDL_Window *window)
    : _window(window), _sdlRenderer(nullptr), _currWidgetCounter(0),
      _isRendererLocked(true) {

}

int32_t Renderer::init() {
  int32_t err = EXIT_SUCCESS;
  const auto initialSize = 250;
  _widgets.resize(initialSize);

  if (EXIT_SUCCESS == err) {
    /** Set texture filtering to linear
     *                      (used for image scaling /pixel interpolation/ )
     * */
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
      LOGERR(
          "Warning: Linear texture filtering not enabled! "
          "SDL_SetHint() failed. SDL Error: %s", SDL_GetError());
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    //Create renderer for window
    _sdlRenderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

    if (nullptr == _sdlRenderer) {
      LOGERR("Renderer could not be created! SDL Error: %s", SDL_GetError());

      err = EXIT_FAILURE;
    } else {
      //Initialize renderer color to blue
      if (EXIT_SUCCESS != SDL_SetRenderDrawColor(_sdlRenderer, 0, 0, 255,
      SDL_ALPHA_OPAQUE)) {
        LOGERR("Error in, SDL_SetRenderDrawColor(), SDL Error: %s",
            SDL_GetError());

        err = EXIT_FAILURE;
      } else {
        Texture::setRenderer(_sdlRenderer);
      }
    }
  }

  return err;
}

void Renderer::deinit() {
  if (_sdlRenderer) //sanity check
  {
    //Destroy window
    SDL_DestroyRenderer(_sdlRenderer);
    _sdlRenderer = nullptr;
  }

  if (_window) //sanity check
  {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
}

void Renderer::clearScreen() {
  //clear screen
  if (EXIT_SUCCESS != SDL_RenderClear(_sdlRenderer)) {
    LOGERR("Error in, SDL_RenderClear(), SDL Error: %s", SDL_GetError());

    return;
  }
}

void Renderer::finishFrame() {
  if (!_isRendererLocked) {
    LOGERR("Error, renderer is left unlocked while finishFrame has been called."
        " Renderer will forcefully lock itself at the cost of breaking "
        "'visually' that forgot to lock it");
    _isRendererLocked = true;
  }

  drawStoredWidgets(_widgets.data(), _currWidgetCounter);

  //reset widget counter
  _currWidgetCounter = 0;


}

void Renderer::drawTexture(const DrawParams *drawParams) {
  if (_currWidgetCounter < _widgets.size()) {
    _widgets[_currWidgetCounter] = *drawParams;
  } else {
    LOGERR("Warning, dynamic resize of Renderer's widget's size. Consider "
        "increasing the initial size of the widgets to prevent run-time spike");
    _widgets.push_back(*drawParams);
  }
  ++_currWidgetCounter;
}

void Renderer::drawTextureArr(const DrawParams drawParamsArr[],
                              const size_t size) {
  if (_currWidgetCounter + size >= _widgets.size()) {
    _widgets.resize((_currWidgetCounter + size) * 2);

    LOGERR("Warning, dynamic resize of Renderer's widget's size. Consider "
        "increasing the initial size of the widgets to prevent run-time spike");
  }

  for (size_t i = 0; i < size; ++i) {
    _widgets[_currWidgetCounter] = drawParamsArr[i];
  }
  _currWidgetCounter += size;
}

int32_t Renderer::lockRenderer() {
  if (_isRendererLocked) {
    LOGERR("Error, renderer is already locked");
    return EXIT_FAILURE;
  }

  _isRendererLocked = true;
  return EXIT_SUCCESS;
}

int32_t Renderer::unlockRenderer() {
  if (!_isRendererLocked) {
    LOGERR("Error, renderer is already unlocked");
    return EXIT_FAILURE;
  }

  _isRendererLocked = false;
  return EXIT_SUCCESS;
}

void Renderer::changeRendererTarget(const int32_t FBOId) {
  if (_isRendererLocked) {
    LOGERR("Error, renderer is locked! changeRendererTarget() for FBOId: %d "
        "will not be performed", FBOId);
    return;
  }

  Texture::setRendererTarget(gRsrcMgr->getFBOTexture(FBOId));
}

void Renderer::resetRendererTarget() {
  if (!_isRendererLocked) {
    LOGERR("Error, renderer is unlocked! resetRendererTarget() will not be "
        "performed");
    return;
  }

  //set default back buffer
  Texture::setRendererTarget(nullptr);
}

void Renderer::updateCurrRendererTarget(const DrawParams drawParamsArr[],
                                        const size_t size) {
  drawStoredWidgets(drawParamsArr, size);
}

void Renderer::drawStoredWidgets(const DrawParams drawParamsArr[],
                                 const size_t size) {
  SDL_Rect renderQuad = { 0, 0, 0, 0 };
  SDL_Rect sourceQuad = { 0, 0, 0, 0 };
  SDL_Texture * texture = nullptr;

  //do the actual drawing of all stored images for THIS FRAME
  for (size_t i = 0; i < size; ++i) {
    sourceQuad = { drawParamsArr[i].frameRect.x, drawParamsArr[i].frameRect.y,
                   drawParamsArr[i].frameRect.w, drawParamsArr[i].frameRect.h };

    renderQuad.x = drawParamsArr[i].pos.x;
    renderQuad.y = drawParamsArr[i].pos.y;
    renderQuad.w = drawParamsArr[i].width;
    renderQuad.h = drawParamsArr[i].height;

    if (WidgetType::IMAGE == drawParamsArr[i].widgetType) {
      texture = gRsrcMgr->getImageTexture(drawParamsArr[i].rsrcId);
    } else if (WidgetType::TEXT == drawParamsArr[i].widgetType) {
      texture = gRsrcMgr->getTextTexture(drawParamsArr[i].textId);
    } else { // (WidgetType::FBO == drawParamsArr[i].widgetType){
      texture = gRsrcMgr->getFBOTexture(drawParamsArr[i].FBOId);
    }

    if (EXIT_SUCCESS !=
        SDL_RenderCopy(_sdlRenderer, texture, &sourceQuad,  &renderQuad)) {
          LOGERR("Error in, SDL_RenderCopy(), SDL Error: %s", SDL_GetError());
          break;
    }
  }

  //------------- UPDATE SCREEN----------------
  SDL_RenderPresent(_sdlRenderer);
}

