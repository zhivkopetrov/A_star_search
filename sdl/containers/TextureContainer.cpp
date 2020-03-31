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
  int32_t err = EXIT_SUCCESS;

  if ( EXIT_SUCCESS != ImageContainer::init(windowWidth, windowHeight)) {
    LOGERR("Error in ImageContainer::init()");
    err = EXIT_FAILURE;
  }

  if ( EXIT_SUCCESS == err) {
    if ( EXIT_SUCCESS != TextContainer::init()) {
      LOGERR("Error in TextContainer::init()");
      err = EXIT_FAILURE;
    }
  }

  if ( EXIT_SUCCESS == err) {
    if ( EXIT_SUCCESS != FBOContainer::init()) {
      LOGERR("Error in TextContainer::init()");
      err = EXIT_FAILURE;
    }
  }

  return err;
}

void TextureContainer::deinit() {
  ImageContainer::deinit();
  TextContainer::deinit();
  FBOContainer::deinit();
}

