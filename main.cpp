/*
 * main.cpp
 *
 *  Created on: 6 Feb 2018
 *      Author: Zhivko Petrov
 */


//C system headers

//C++ system headers
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <cstring>

//Other libraries headers
#include "sdl/SDLLoader.h"

//Own components headers
#include "Game.h"


int32_t main(int32_t argc, char * args[])
{
    int32_t err = EXIT_SUCCESS;

    bool isDiagonalMovementAllowed = false;
    if(1 < argc)
    {
        if(0 == strcmp(args[1], "true"))
        {
            isDiagonalMovementAllowed = true;
        }
    }

    Game game;

    if(EXIT_SUCCESS != SDLLoader::init())
    {
        fprintf(stderr, "Error in SDLLoader::init() -> Terminating ...\n");

        err = EXIT_FAILURE;
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != game.init(isDiagonalMovementAllowed))
        {
            fprintf(stderr, "game.init() failed\n");

            err = EXIT_FAILURE;
        }
        else
        {
            game.start();
        }
    }

    game.deinit();

    //close SDL libraries
    SDLLoader::deinit();

    return err;
}

