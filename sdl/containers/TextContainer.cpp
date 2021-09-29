//Corresponding header
#include "TextContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL_ttf.h>

//Own components headers
#include "common/CommonDefines.h"
#include "sdl/Texture.h"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

TextContainer::TextContainer()
    : _fontSmall(nullptr), _fontBig(nullptr) {
}

int32_t TextContainer::init() {
  if (EXIT_SUCCESS != loadFonts()) {
    LOGERR("Error, loadFonts() failed");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void TextContainer::deinit() {
  if (_fontSmall) //sanity check
  {
    TTF_CloseFont(_fontSmall);
    _fontSmall = nullptr;
  }

  if (_fontBig) //sanity check
  {
    TTF_CloseFont(_fontBig);
    _fontBig = nullptr;
  }

  for (SDL_Texture *texture : _textures) {
    Texture::freeTexture(texture);
  }
  _textures.clear();
}

void TextContainer::createText(const char *text, const Color &color,
                               const int32_t fontSize, int32_t &outTextId,
                               int32_t &outTextWidth, int32_t &outTextHeight) {
  TTF_Font *font = (FontSize::SMALL == fontSize) ? _fontSmall : _fontBig;
  occupyFreeSlotIndex(outTextId);

  //create hardware accelerated texture
  if ( EXIT_SUCCESS
      != Texture::loadFromText(text, font, color, _textures[outTextId],
          outTextWidth, outTextHeight)) {
    LOGERR("Unable to load text: %s with ID: %d", text, outTextId);
    freeSlotIndex(outTextId);
  }
}

void TextContainer::reloadText(const char *text, const Color &color,
                               const int32_t fontSize, const int32_t textId,
                               int32_t &outTextWidth, int32_t &outTextHeight) {
  TTF_Font *font = (FontSize::SMALL == fontSize) ? _fontSmall : _fontBig;

  //create hardware accelerated texture
  if ( EXIT_SUCCESS
      != Texture::loadFromText(text, font, color, _textures[textId],
          outTextWidth, outTextHeight)) {
    LOGERR("Unable to load text: %s with ID: %d", text, textId);
    freeSlotIndex(textId);
  }
}

void TextContainer::unloadText(const int32_t textId) {
  if (textId >= static_cast<int32_t>(_textures.size())) {
    LOGERR("Warning, unloading a non-existing text index for textId: %d",
        textId);
    return;
  }
  freeSlotIndex(textId);
}

void TextContainer::occupyFreeSlotIndex(int32_t &occupiedTextId) {
  bool foundIdx = false;

  const int32_t size = static_cast<int32_t>(_textures.size());
  for (int32_t idx = 0; idx < size; ++idx) {
    if (nullptr == _textures[idx]) {
      occupiedTextId = idx;
      foundIdx = true;
      break;
    }
  }

  //create additional space
  if (!foundIdx) {
    _textures.push_back(nullptr);
    occupiedTextId = size;
  }
}

void TextContainer::freeSlotIndex(const int32_t textId) {
  Texture::freeTexture(_textures[textId]);
}

int32_t TextContainer::loadFonts() {
  int32_t err = EXIT_SUCCESS;

  //Open the font with font size 30
  _fontSmall = TTF_OpenFont("../assets/orbitron-medium.otf", 50);
  if (nullptr == _fontSmall) {
    LOGERR("Failed to load font! SDL_ttf Error: %s", TTF_GetError());

    err = EXIT_FAILURE;
  }

  if ( EXIT_SUCCESS == err) {
    //Open the font with font size 80
    _fontBig = TTF_OpenFont("../assets/orbitron-medium.otf", 80);
    if (nullptr == _fontBig) {
      LOGERR("Failed to load font! SDL_ttf Error: %s", TTF_GetError());

      err = EXIT_FAILURE;
    }
  }

  return err;
}

