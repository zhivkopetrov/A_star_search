//Corresponding header
#include "TextureContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers


//Own components headers
#include "common/CommonDefines.h"
#include "sdl/Texture.h"
#include "utils/Log.h"

TextureContainer::TextureContainer() {

}

int32_t TextureContainer::init(const int32_t windowWidth,
                               const int32_t windowHeight) {
  if ( EXIT_SUCCESS != ImageContainer::init(windowWidth, windowHeight)) {
    LOGERR("Error in ImageContainer::init()");
    return EXIT_FAILURE;
  }

  if ( EXIT_SUCCESS != TextContainer::init()) {
    LOGERR("Error in TextContainer::init()");
    return EXIT_FAILURE;
  }

  if ( EXIT_SUCCESS != FBOContainer::init()) {
    LOGERR("Error in TextContainer::init()");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void TextureContainer::deinit() {
  ImageContainer::deinit();
  TextContainer::deinit();
  FBOContainer::deinit();
}

