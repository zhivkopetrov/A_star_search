//C system headers

//C++ system headers
#include <cstdint>
#include <cstdlib>
#include <cstring>

//Other libraries headers
#include "sdl/SDLLoader.h"

//Own components headers
#include "Engine.h"
#include "utils/Log.h"

static int32_t runApplication() {
  Engine engine;

  if (EXIT_SUCCESS != engine.init()) {
    LOGERR("engine.init() failed");
    return EXIT_FAILURE;
  }

  engine.start();

  engine.deinit();

  return EXIT_SUCCESS;
}

int32_t main() {
  if (EXIT_SUCCESS != SDLLoader::init()) {
    LOGERR("Error in SDLLoader::init() -> Terminating ...");
    return EXIT_FAILURE;
  }

  if (EXIT_SUCCESS != runApplication()) {
    LOGERR("runApplication() failed");
    return EXIT_FAILURE;
  }

  //close SDL libraries
  SDLLoader::deinit();

  return EXIT_SUCCESS;
}

