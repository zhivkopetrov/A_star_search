//Corresponding header
#include "GameCursor.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include "sdl/MouseUtils.h"

//Own components headers
#include "utils/Log.h"

GameCursor::~GameCursor() {
  deinit();
}

int32_t GameCursor::init(const char *imagePath, const int32_t cursorClickX,
                         const int32_t cursorClickY) {
  if (EXIT_SUCCESS != MouseUtils::createCursorFromImg(imagePath, cursorClickX,
          cursorClickY)) {
    LOGERR("MouseUtils::createCursorFromImg() failed for imagePath: %s",
        imagePath);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void GameCursor::deinit() {
  MouseUtils::freeCursor();
}
