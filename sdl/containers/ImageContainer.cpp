//Corresponding header
#include "ImageContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "sdl/Texture.h"
#include "utils/Log.h"

ImageContainer::ImageContainer() {
  for (SDL_Texture *&texture : _textures) {
    texture = nullptr;
  }
}

int32_t ImageContainer::init(const int32_t windowWidth,
                             const int32_t windowHeight) {
  int32_t err = EXIT_SUCCESS;

  populateTextureFrameRects(windowWidth, windowHeight);

  if (EXIT_SUCCESS != loadTextures()) {
    LOGERR("Error, loadTextures() failed");
    err = EXIT_FAILURE;
  }

  return err;
}

void ImageContainer::deinit() {
  for (SDL_Texture *texture : _textures) {
    Texture::freeTexture(texture);
  }
}

int32_t ImageContainer::loadTextures() {
  int32_t err = EXIT_SUCCESS;

  const std::pair<const char*, int32_t> assets[] = { {
      "../assets/vertical_line.png", Textures::VERTICAL_LINE }, {
      "../assets/horizontal_line.png", Textures::HORIZONTAL_LINE }, {
      "../assets/start.png", Textures::START_NODE }, { "../assets/end.png",
      Textures::END_NODE }, { "../assets/water.png", Textures::WALL }, {
      "../assets/batman_symbol.png", Textures::A_STAR_PATH }, {
      "../assets/batman_big.png", Textures::BATMAN_BIG }, {
      "../assets/batman_small.png", Textures::BATMAN_SMALL }, {
      "../assets/win_dialog.png", Textures::WIN_DIALOG }, {
      "../assets/lose_dialog.png", Textures::LOSE_DIALOG }, {
      "../assets/obstacles.png", Textures::OBSTACLES }, {
      "../assets/ui/check_frames.png", Textures::MENU_TICK } };

  for (int32_t i = Textures::VERTICAL_LINE; i <= Textures::MENU_TICK; ++i) {
    if ( EXIT_SUCCESS != loadSingleTexture(assets[i].first, assets[i].second)) {
      LOGERR("Error in loadSingleTexture()");

      err = EXIT_FAILURE;
      break;
    }
  }

  return err;
}

int32_t ImageContainer::loadSingleTexture(const char *filePath,
                                          const int32_t textureId) {
  int32_t err = EXIT_SUCCESS;

  SDL_Surface *surface = nullptr;

  if (EXIT_SUCCESS != Texture::loadSurfaceFromFile(filePath, surface)) {
    LOGERR("Unable to load image %s! ", filePath);
    err = EXIT_FAILURE;
  } else {
    //create hardware accelerated texture
    if ( EXIT_SUCCESS
        != Texture::loadTextureFromSurface(surface, _textures[textureId])) {
      LOGERR("Unable to create texture with Id: %d", textureId);

      err = EXIT_FAILURE;
    }
  }

  return err;
}

void ImageContainer::populateTextureFrameRects(const int32_t windowWidth,
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

   _textureFrameRects[Textures::MENU_TICK].resize(2);
   for (int32_t i = 0; i < 2; ++i) {
     _textureFrameRects[Textures::MENU_TICK][i].x = i * 33;
     _textureFrameRects[Textures::MENU_TICK][i].w = 33;
     _textureFrameRects[Textures::MENU_TICK][i].h = 33;
   }
}

