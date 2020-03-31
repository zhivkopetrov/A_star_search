//Corresponding header
#include "FBOContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "sdl/Texture.h"
#include "utils/Log.h"

int32_t FBOContainer::init() {
  return EXIT_SUCCESS;
}

void FBOContainer::deinit() {
  for (SDL_Texture *texture : _textures) {
    Texture::freeTexture(texture);
  }
  _textures.clear();
}

void FBOContainer::createFBO(const int32_t FBOWidth, const int32_t FBOHeight,
                             int32_t &outFBOId) {
  occupyFreeSlotIndex(outFBOId);

  //create hardware accelerated texture
  if ( EXIT_SUCCESS
      != Texture::createEmptyTexture(FBOWidth, FBOHeight,
          _textures[outFBOId])) {
    LOGERR("Unable to create FBO: with ID: %d", outFBOId);
    destroyFBO(outFBOId);
  }
}

void FBOContainer::destroyFBO(const int32_t FBOId) {
  Texture::freeTexture(_textures[FBOId]);
}

void FBOContainer::occupyFreeSlotIndex(int32_t &occupiedTextId) {
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



