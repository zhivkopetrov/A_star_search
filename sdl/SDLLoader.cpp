//Corresponding header
#include "SDLLoader.h"

//C system headers

//C++ system headers
#include <cstdio>

//Other libraries headers
#include <SDL.h>
#include <SDL_video.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

//Own components headers
#include "utils/Log.h"

int32_t SDLLoader::init() {
  if (-1 == TTF_Init()) {
    LOGERR("SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());

    return EXIT_FAILURE;
  }

  if (0 > SDL_Init(SDL_INIT_VIDEO)) {
    LOGERR("SDL could not be initialised! SDL Error: %s", SDL_GetError());

    return EXIT_FAILURE;
  }

  //Initialise PNG loading
  const int32_t imgFlags = IMG_INIT_PNG;
  if (! (IMG_Init(imgFlags) & imgFlags)) {
    LOGERR("SDL_image could not be initialised! SDL_image Error: %s",
        IMG_GetError());

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void SDLLoader::deinit() {
  //Quit SDL subsystems
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

