//Corresponding header
#include "Renderer.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstring>

//Other libraries headers
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_hints.h>

//Own components headers
#include "Texture.h"
#include "managers/RsrcMgr.h"
#include "utils/Log.h"

Renderer::Renderer(SDL_Window *window)
    : _window(window), _sdlRenderer(nullptr), _currWidgetCounter(0) {

}

int32_t Renderer::init() {
  int32_t err = EXIT_SUCCESS;

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
  SDL_Rect renderQuad = { 0, 0, 0, 0 };
  SDL_Rect sourceQuad = { 0, 0, 0, 0 };
  SDL_Texture * texture = nullptr;

  const uint32_t USED_SIZE = _currWidgetCounter;
  //reset widget counter
  _currWidgetCounter = 0;

  //do the actual drawing of all stored images for THIS FRAME
  for (uint32_t i = 0; i < USED_SIZE; ++i) {
    sourceQuad = { _widgets[i].frameRect.x, _widgets[i].frameRect.y,
                   _widgets[i].frameRect.w, _widgets[i].frameRect.h };

    renderQuad.x = _widgets[i].pos.x;
    renderQuad.y = _widgets[i].pos.y;
    renderQuad.w = _widgets[i].width;
    renderQuad.h = _widgets[i].height;

    if (WidgetType::IMAGE == _widgets[i].widgetType) {
      texture = gRsrcMgr->getImageTexture(_widgets[i].rsrcId);
    } else if (WidgetType::TEXT == _widgets[i].widgetType) {
      texture = gRsrcMgr->getTextTexture(_widgets[i].textId);
    } else { // (WidgetType::FBO == _widgets[i].widgetType){
      //TODO populate me
      LOGERR("Error, wrong widget type detected");
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

void Renderer::drawTexture(DrawParams *drawParams) {
  _widgets[_currWidgetCounter] = *drawParams;

  ++_currWidgetCounter;
}

void Renderer::drawTextureArr(DrawParams drawParamsArr[], const int32_t size) {
  memcpy(&_widgets[_currWidgetCounter], drawParamsArr,
      sizeof(DrawParams) * size);

  _currWidgetCounter += size;
}

