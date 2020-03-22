//C system headers
//C++ system headers
#include <cstdint>
#include <cstdlib>
#include <cstring>

//Other libraries headers
#include "sdl/SDLLoader.h"

//Own components headers
#include "Game.h"
#include "utils/Log.h"

int32_t main(int32_t argc, char *args[]) {
  int32_t err = EXIT_SUCCESS;
  Game game;

  bool isDiagonalMovementAllowed = false;
  if (1 < argc) {
    if (0 == strcmp(args[1], "true")) {
      isDiagonalMovementAllowed = true;
    }
  }

  if (EXIT_SUCCESS != SDLLoader::init()) {
    LOGERR("Error in SDLLoader::init() -> Terminating ...");

    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != game.init(isDiagonalMovementAllowed)) {
      LOGERR("game.init() failed");

      err = EXIT_FAILURE;
    } else {
      game.start();
    }
  }

  game.deinit();

  //close SDL libraries
  SDLLoader::deinit();

  return err;
}

