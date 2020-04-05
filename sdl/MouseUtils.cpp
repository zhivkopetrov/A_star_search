//Corresponding header
#include "MouseUtils.h"

//C system headers

//C++ system headers

//Other libraries headers
#include <SDL_mouse.h>
#include <SDL_image.h>

//Own components headers
#include "utils/Log.h"

static SDL_Cursor *cursor = nullptr;

int32_t MouseUtils::toggleCursorStatus(const int32_t cursorStatus) {
  return SDL_ShowCursor(cursorStatus);
}

int32_t MouseUtils::createCursorFromImg(const char *imagePath,
                                        const int32_t cursorClickX,
                                        const int32_t cursorClickY) {
  int32_t err = EXIT_SUCCESS;

  SDL_Surface *cursorSurface = IMG_Load(imagePath);

  if (nullptr == cursorSurface) {
    LOGERR("Unable to load image %s! SDL_image Error: %s", imagePath,
        IMG_GetError());
    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    cursor = SDL_CreateColorCursor(cursorSurface, cursorClickX, cursorClickY);

    if (nullptr == cursor) {
      LOGERR("Unable to create color cursor! SDL_image Error: %s",
          IMG_GetError());
      err = EXIT_FAILURE;
    } else {
      SDL_SetCursor(cursor);
      SDL_FreeSurface(cursorSurface);
    }
  }

  return err;
}

void MouseUtils::freeCursor() {
  if (cursor) //sanity check
  {
    SDL_FreeCursor(cursor);
    cursor = nullptr;
  }
}

