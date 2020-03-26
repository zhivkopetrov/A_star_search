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

int32_t main(const int32_t , char *[]) {
  int32_t err = EXIT_SUCCESS;
  Engine engine;

  if (EXIT_SUCCESS != SDLLoader::init()) {
    LOGERR("Error in SDLLoader::init() -> Terminating ...");

    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != engine.init()) {
      LOGERR("engine.init() failed");

      err = EXIT_FAILURE;
    } else {
      engine.start();
    }
  }

  engine.deinit();

  //close SDL libraries
  SDLLoader::deinit();

  return err;
}

