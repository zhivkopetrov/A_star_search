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
#include "utils/EnumClassUtils.hpp"
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

  if (EXIT_SUCCESS !=
      Texture::setRendererTarget(gRsrcMgr->getFBOTexture(FBOId))) {
    LOGERR("Texture::setRendererTarget() failed for FBO with Id: %d", FBOId);
  }
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

void Renderer::clearCurrentRendererTarget(const Color &clearColor) {
  if (EXIT_SUCCESS != Texture::clearCurrentRendererTarget(clearColor)) {
    LOGERR("Texture::clearCurrentRendererTarget() failed");
  }
}

void Renderer::updateCurrRendererTarget(const DrawParams drawParamsArr[],
                                        const size_t size) {
  drawStoredWidgets(drawParamsArr, size);
}

void Renderer::setWidgetBlendMode(const DrawParams &widgetInfo,
                                  const BlendMode blendMode) {
  SDL_Texture * texture = getTargetTexture(widgetInfo);

  if (EXIT_SUCCESS !=
      Texture::setBlendMode(texture, getEnumClassValue(blendMode))) {
    LOGERR("Error, Texture::setBlendMode() failed for widget with Id: %d",
        widgetInfo.rsrcId);
  }
}

void Renderer::setWidgetOpacity(const DrawParams &widgetInfo,
                                const uint8_t opacity) {
  SDL_Texture * texture = getTargetTexture(widgetInfo);

  if (EXIT_SUCCESS != Texture::setAlpha(texture, opacity)) {
    LOGERR("Error, Texture::setAlpha() failed for widget with Id: %d",
        widgetInfo.rsrcId);
  }
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
    renderQuad = { drawParamsArr[i].pos.x, drawParamsArr[i].pos.y,
                   drawParamsArr[i].width, drawParamsArr[i].height };

    texture = getTargetTexture(drawParamsArr[i]);
    //images have additional pre-draw and post-draw evaluations
    if (WidgetType::IMAGE == drawParamsArr[i].widgetType) {
      drawImageTexture(
            texture, sourceQuad, renderQuad, drawParamsArr[i].opacity);
      continue;
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

void Renderer::drawImageTexture(SDL_Texture *texture,
                                const SDL_Rect &sourceQuad,
                                const SDL_Rect &renderQuad,
                                const int32_t imageOpacity) {
  /* Since multiple WidgetType::IMAGE widget can reused the same texture -
   * if opacity is present:
   * - change the opacity
   * - make the draw
   * - restore the original opacity
   * */
  if (FULL_OPACITY == imageOpacity) {
    if (EXIT_SUCCESS !=
        SDL_RenderCopy(_sdlRenderer, texture, &sourceQuad,  &renderQuad)) {
          LOGERR("Error in, SDL_RenderCopy(), SDL Error: %s", SDL_GetError());
          return;
    }
  } else {
    Texture::setAlpha(texture, static_cast<uint8_t>(imageOpacity));
    if (EXIT_SUCCESS !=
        SDL_RenderCopy(_sdlRenderer, texture, &sourceQuad,  &renderQuad)) {
          LOGERR("Error in, SDL_RenderCopy(), SDL Error: %s", SDL_GetError());
          return;
    }
    Texture::setAlpha(texture, static_cast<uint8_t>(FULL_OPACITY));
  }
}

SDL_Texture * Renderer::getTargetTexture(const DrawParams &widgetInfo) {
  SDL_Texture * texture = nullptr;
  if (WidgetType::IMAGE == widgetInfo.widgetType) {
    texture = gRsrcMgr->getImageTexture(widgetInfo.rsrcId);
  } else if (WidgetType::TEXT == widgetInfo.widgetType) {
    texture = gRsrcMgr->getTextTexture(widgetInfo.textId);
  } else { // (WidgetType::FBO == widgetInfo.widgetType){
    texture = gRsrcMgr->getFBOTexture(widgetInfo.FBOId);
  }
  return texture;
}

