//Corresponding header
#include "Texture.h"

//C system headers

//C++ system headers

//Other libraries headers
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

//Own components headers
#include "utils/drawing/Color.h"
#include "utils/Log.h"

SDL_Renderer *Texture::_renderer = nullptr;

void Texture::freeSurface(SDL_Surface *&surface) {
  if (surface) //sanity check
  {
    SDL_FreeSurface(surface);
    surface = nullptr;
  }
}

void Texture::freeTexture(SDL_Texture *&texture) {
  if (texture) //sanity check
  {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
}

int32_t Texture::loadSurfaceFromFile(const char *path,
                                     SDL_Surface *&outSurface) {
  freeSurface(outSurface);

  //Load image at specified path
  outSurface = IMG_Load(path);
  if (nullptr == outSurface) {
    LOGERR("Unable to load image %s! SDL_image Error: %s", path,
        IMG_GetError());

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int32_t Texture::loadFromText(const char *text,
                              TTF_Font *font,
                              const Color& color,
                              SDL_Texture *& outTexture,
                              int32_t &outTextWidth,
                              int32_t &outTextHeight) {
  freeTexture(outTexture);

  SDL_Surface *loadedSurface =
      TTF_RenderText_Blended(font, text,
          *(reinterpret_cast<const SDL_Color *>(&color.rgba)));

  if (loadedSurface == nullptr) {
    LOGERR("Unable to load image! SDL_image Error: %s", IMG_GetError());

    return EXIT_FAILURE;
  }

  outTextWidth = loadedSurface->w;
  outTextHeight = loadedSurface->h;

  //create hardware accelerated texture
  if ( EXIT_SUCCESS
      != Texture::loadTextureFromSurface(loadedSurface, outTexture)) {
    LOGERR("Unable to create text texture");

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int32_t Texture::loadTextureFromSurface(SDL_Surface *&surface,
                                        SDL_Texture *&outTexture) {
  //Create texture from surface pixels
  outTexture = SDL_CreateTextureFromSurface(_renderer, surface);

  if (nullptr == outTexture) {
    LOGERR("Unable to create texture! SDL Error: %s", SDL_GetError());

    return EXIT_FAILURE;
  } else //success
  {
    SDL_FreeSurface(surface);
    surface = nullptr;
  }

  return EXIT_SUCCESS;
}

int32_t Texture::createEmptyTexture(const int32_t  width,
                                    const int32_t  height,
                                    SDL_Texture *& outTexture) {
  if(nullptr != outTexture) {
      LOGERR("Warning, outTexture is not empty. Will not create Empty "
             "Surface. Memory leak prevented.");
      return EXIT_FAILURE;
  }

  /** NOTE: This method will probably be used by Hardware Accelerated
   *        FBO's, whose Texture's are going to be temporary
   *        targets for the Hardware Accelerated Renderer.
   *        In order for this to work, the SDL_Texture must be created
   *        with access specifier: SDL_TEXTUREACCESS_TARGET.
   *
   * NOTE2: For empty surface format - 32 bit depth format [RGBA] is used
   * */
  outTexture = SDL_CreateTexture(_renderer,                //hardware renderer
                                 SDL_PIXELFORMAT_RGBA8888, //format
                                 SDL_TEXTUREACCESS_TARGET, //access
                                 width,                    //texture width
                                 height);                  //texture height

  if(nullptr == outTexture) {
    LOGERR("SDL_CreateTexture() failed: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int32_t Texture::clearCurrentRendererTarget(const Color & clearColor)
{
  Color currRendererColor = Colors::BLACK;
  bool isSameColorAsOld = true;

  //remember old renderer color
  if(EXIT_SUCCESS != SDL_GetRenderDrawColor(_renderer,
                                            &currRendererColor.rgba.r,
                                            &currRendererColor.rgba.g,
                                            &currRendererColor.rgba.b,
                                            &currRendererColor.rgba.a)) {
    LOGERR("Error in, SDL_GetRenderDrawColor(), SDL Error: %s",
                                                         SDL_GetError());
    return EXIT_FAILURE;
  }

  if(currRendererColor != clearColor) {
      isSameColorAsOld = false;
  }

  if(isSameColorAsOld) {
    //clear target
    if(EXIT_SUCCESS != SDL_RenderClear(_renderer))  {
        LOGERR("Error in, SDL_RenderClear(), SDL Error: %s", SDL_GetError());
        return EXIT_FAILURE;
    }
  }
  else {//color should be changed temporary
    //set new renderer color
    if(EXIT_SUCCESS != SDL_SetRenderDrawColor(_renderer,
                                              clearColor.rgba.r,
                                              clearColor.rgba.g,
                                              clearColor.rgba.b,
                                              clearColor.rgba.a)) {
      LOGERR("Error in, SDL_SetRenderDrawColor(), SDL Error: %s",
                                                   SDL_GetError());
      return EXIT_FAILURE;
    }

    //clear target
    if(EXIT_SUCCESS != SDL_RenderClear(_renderer)) {
      LOGERR("Error in, SDL_RenderClear(), SDL Error: %s", SDL_GetError());
      return EXIT_FAILURE;
    }

    //restore old renderer colour

    if(EXIT_SUCCESS !=
            SDL_SetRenderDrawColor(_renderer,
                                   currRendererColor.rgba.r,
                                   currRendererColor.rgba.g,
                                   currRendererColor.rgba.b,
                                   currRendererColor.rgba.a)) {
      LOGERR("Error in, SDL_SetRenderDrawColor(), SDL Error: %s",
                                                   SDL_GetError());
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

int32_t Texture::setAlpha(SDL_Texture * texture, const uint8_t alpha) {
  if(EXIT_SUCCESS !=  SDL_SetTextureAlphaMod(texture, alpha)) {
    LOGERR("Warning, .setAlpha() method will not take effect. Reason: "
           "invalid texture or alpha modulation is not supported. "
          "SDL_SetTextureAlphaMod() failed. SDL Error: %s", SDL_GetError());
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int32_t Texture::setBlendMode(SDL_Texture * texture, const uint8_t blendMode) {
  if(EXIT_SUCCESS !=
          SDL_SetTextureBlendMode(texture,
                                  static_cast<SDL_BlendMode>(blendMode))) {
    LOGERR("Warning, .setBlendMode() method will not take effect. Reason: "
           "invalid texture or blend mode is not supported. "
           "SDL_SetTextureBlendMode() failed. SDL Error: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int32_t Texture::setRendererTarget(SDL_Texture * target) {
  int32_t err = EXIT_SUCCESS;

  if(nullptr == _renderer) {
      LOGERR("Error, renderer is still not set for Texture. You are missing"
                      " Texture::setRenderer() call "
                                  "in the program initialization process");
      err = EXIT_FAILURE;
  }

  if(EXIT_SUCCESS == err) {
      if(EXIT_SUCCESS != SDL_SetRenderTarget(_renderer, target)) {
          LOGERR("Error, default renderer target could not be set. "
                          "SDL_SetRenderTarget() failed, SDL Error: %s",
                                                             SDL_GetError());
          err = EXIT_FAILURE;
      }
  }
  return err;
}

void Texture::setRenderer(SDL_Renderer *renderer) {
  _renderer = renderer;
}

