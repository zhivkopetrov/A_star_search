//Corresponding header
#include "TextureContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_ttf.h>

//Own components headers
#include "common/CommonDefines.h"
#include "sdl/Texture.h"
#include "utils/Log.h"

TextureContainer::TextureContainer()
    : _fontSmall(nullptr), _fontBig(nullptr) {

  //set green color for text
  _color.r = 0;
  _color.g = 255;
  _color.b = 0;
  _color.a = 255;
}

int32_t TextureContainer::init(const int32_t windowWidth,
                               const int32_t windowHeight) {
  int32_t err = EXIT_SUCCESS;

  _textures.resize(Textures::COUNT, nullptr);

  _textureFrameRects.resize(Textures::COUNT);

  if ( EXIT_SUCCESS != loadFonts()) {
    LOGERR("Error in loadFonts()");

    err = EXIT_FAILURE;
  }

  if ( EXIT_SUCCESS == err) {
    if ( EXIT_SUCCESS != loadTextures(windowWidth, windowHeight)) {
      LOGERR("Error in loadTextures()");

      err = EXIT_FAILURE;
    }
  }

  return err;
}

void TextureContainer::deinit() {
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
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }

  _textures.clear();
}

void TextureContainer::setText(const char *text, const int32_t fontSize,
                               const uint8_t textureId, int32_t *outTextWidth,
                               int32_t *outTextHeight) {
  TTF_Font *font = nullptr;

  FontSize::SMALL == fontSize ? font = _fontSmall : font = _fontBig;

  //create hardware accelerated texture
  if ( EXIT_SUCCESS
      != Texture::loadFromText(text, font, _color, _textures[textureId],
          outTextWidth, outTextHeight)) {
    LOGERR("Unable to load text: %s", text);

    return;
  }

  _textureFrameRects[textureId].w = *outTextWidth;
  _textureFrameRects[textureId].h = *outTextHeight;
}

int32_t TextureContainer::loadFonts() {
  int32_t err = EXIT_SUCCESS;

  //Open the font with font size 30
  _fontSmall = TTF_OpenFont("../assets/orbitron-medium.otf", 30);
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

int32_t TextureContainer::loadTextures(const int32_t windowWidth,
                                       const int32_t windowHeight) {
  int32_t err = EXIT_SUCCESS;

  populateTextureFrameRects(windowWidth, windowHeight);

  const std::pair<const char*, uint8_t> assets[] = { {
      "../assets/vertical_line.png", Textures::VERTICAL_LINE }, {
      "../assets/horizontal_line.png", Textures::HORIZONTAL_LINE }, {
      "../assets/start.png", Textures::START_NODE }, { "../assets/end.png",
      Textures::END_NODE }, { "../assets/wall.png", Textures::WALL }, {
      "../assets/path.png", Textures::A_STAR_PATH } };

  for (uint8_t i = Textures::VERTICAL_LINE; i <= Textures::A_STAR_PATH; ++i) {
    if ( EXIT_SUCCESS != loadSingleTexture(assets[i].first, assets[i].second)) {
      LOGERR("Error in loadSingleTexture()");

      err = EXIT_FAILURE;
      break;
    }
  }

  return err;
}

int32_t TextureContainer::loadSingleTexture(const char *filePath,
                                            const uint8_t textureId) {
  int32_t err = EXIT_SUCCESS;

  SDL_Surface *surface = nullptr;

  if (EXIT_SUCCESS != Texture::loadSurfaceFromFile(filePath, surface)) {
    LOGERR("Unable to load image %s! ", filePath);
    err = EXIT_FAILURE;
  } else {
    //create hardware accelerated texture
    if ( EXIT_SUCCESS
        != Texture::loadTextureFromSurface(surface, _textures[textureId])) {
      LOGERR("Unable to create texture with Id: %hhu", textureId);

      err = EXIT_FAILURE;
    }
  }

  return err;
}

void TextureContainer::populateTextureFrameRects(const int32_t windowWidth,
                                                 const int32_t windowHeight) {
  for (int32_t i = 0; i < Textures::COUNT; ++i) {
    //and it tart from the beginning of the image
    _textureFrameRects[i].x = 0;
    _textureFrameRects[i].y = 0;
  }

  _textureFrameRects[Textures::VERTICAL_LINE].w = 5;
  _textureFrameRects[Textures::VERTICAL_LINE].h = windowHeight;

  _textureFrameRects[Textures::HORIZONTAL_LINE].w = windowWidth;
  _textureFrameRects[Textures::HORIZONTAL_LINE].h = 5;

  constexpr int32_t TILE_DIMENSIONS = 55;

  for (int32_t i = Textures::START_NODE; i <= Textures::A_STAR_PATH; ++i) {
    _textureFrameRects[i].w = TILE_DIMENSIONS;
    _textureFrameRects[i].h = TILE_DIMENSIONS;
  }
}

