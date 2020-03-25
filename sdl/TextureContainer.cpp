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

  //set black color for text
  _color.r = 0;
  _color.g = 0;
  _color.b = 0;
  _color.a = 0;
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

  _textureFrameRects[textureId][0].w = *outTextWidth;
  _textureFrameRects[textureId][0].h = *outTextHeight;
}

int32_t TextureContainer::loadFonts() {
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

int32_t TextureContainer::loadTextures(const int32_t windowWidth,
                                       const int32_t windowHeight) {
  int32_t err = EXIT_SUCCESS;

  populateTextureFrameRects(windowWidth, windowHeight);

  const std::pair<const char*, uint8_t> assets[] = { {
      "../assets/vertical_line.png", Textures::VERTICAL_LINE }, {
      "../assets/horizontal_line.png", Textures::HORIZONTAL_LINE }, {
      "../assets/start.png", Textures::START_NODE }, { "../assets/end.png",
      Textures::END_NODE }, { "../assets/water.png", Textures::WALL }, {
      "../assets/batman_symbol.png", Textures::A_STAR_PATH }, {
      "../assets/batman_big.png", Textures::BATMAN_BIG }, {
      "../assets/batman_small.png", Textures::BATMAN_SMALL }, {
      "../assets/win_dialog.png", Textures::WIN_DIALOG }, {
      "../assets/lose_dialog.png", Textures::LOSE_DIALOG }, {
      "../assets/obstacles.png", Textures::OBSTACLES } };

  for (uint8_t i = Textures::VERTICAL_LINE; i <= Textures::OBSTACLES; ++i) {
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
    //most textures have single frame
    _textureFrameRects[i].resize(1);
    //start from the beginning of the image
    _textureFrameRects[i][0].x = 0;
    _textureFrameRects[i][0].y = 0;
  }

  _textureFrameRects[Textures::OBSTACLES].resize(Grid::OBSTACLES_TYPE_COUNT);
  for (int32_t i = 0; i < Grid::OBSTACLES_TYPE_COUNT; ++i) {
    _textureFrameRects[Textures::OBSTACLES][i].x = i * Grid::TILE_DIMENSION;
    _textureFrameRects[Textures::OBSTACLES][i].w = Grid::TILE_DIMENSION;
    _textureFrameRects[Textures::OBSTACLES][i].h = Grid::TILE_DIMENSION;
  }

  _textureFrameRects[Textures::VERTICAL_LINE][0].w = Grid::LINE_OFFSET;
  _textureFrameRects[Textures::VERTICAL_LINE][0].h = windowHeight;

  _textureFrameRects[Textures::HORIZONTAL_LINE][0].w = windowWidth;
  _textureFrameRects[Textures::HORIZONTAL_LINE][0].h = Grid::LINE_OFFSET;

  for (int32_t i = Textures::START_NODE; i <= Textures::A_STAR_PATH; ++i) {
    _textureFrameRects[i][0].w = Grid::TILE_DIMENSION;
    _textureFrameRects[i][0].h = Grid::TILE_DIMENSION;
  }

  _textureFrameRects[Textures::BATMAN_BIG][0].w =
      BatmanDimensions::BIG_BATMAN_WIDTH;
  _textureFrameRects[Textures::BATMAN_BIG][0].h =
      BatmanDimensions::BIG_BATMAN_HEIGHT;

  _textureFrameRects[Textures::BATMAN_SMALL][0].w =
      BatmanDimensions::SMALL_BATMAN_WIDTH;
  _textureFrameRects[Textures::BATMAN_SMALL][0].h =
      BatmanDimensions::SMALL_BATMAN_HEIGHT;

  _textureFrameRects[Textures::WIN_DIALOG][0].w =
      SpeechDimensions::WIN_DIALOG_WIDTH;
  _textureFrameRects[Textures::WIN_DIALOG][0].h =
      SpeechDimensions::WIN_DIALOG_HEIGHT;

  _textureFrameRects[Textures::LOSE_DIALOG][0].w =
      SpeechDimensions::LOSE_DIALOG_WIDTH;
  _textureFrameRects[Textures::LOSE_DIALOG][0].h =
      SpeechDimensions::LOSE_DIALOG_HEIGHT;
}

