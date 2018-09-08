/*
 * Renderer.cpp
 *
 *  Created on: 6 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "Renderer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_hints.h>

//Own components headers
#include "DrawParams.h"


Renderer::Renderer() : _window(nullptr),
                       _sdlRenderer(nullptr),
                       _currWidgetCounter(0)
{

}

int32_t Renderer::init(const int32_t windowX,
                       const int32_t windowY,
                       const int32_t windowWidth,
                       const int32_t windowHeight)
{
    int32_t err = EXIT_SUCCESS;

    //Create window
    _window = SDL_CreateWindow("BeerC Game",
                               windowX,
                               windowY,
                               windowWidth,
                               windowHeight,
                               SDL_WINDOW_FULLSCREEN_DESKTOP);

    if(nullptr == _window)
    {
        fprintf(stderr, "Window could not be created! SDL Error: %s\n",
                                                               SDL_GetError());

        err = EXIT_FAILURE;
    }

    if(EXIT_SUCCESS == err)
    {
        /** Set texture filtering to linear
         *                      (used for image scaling /pixel interpolation/ )
         * */
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            fprintf(stderr, "Warning: Linear texture filtering not enabled! "
                      "SDL_SetHint() failed. SDL Error: %s\n", SDL_GetError());

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        //Create renderer for window
        _sdlRenderer =
                    SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

        if(nullptr == _sdlRenderer)
        {
            fprintf(stderr, "Renderer could not be created! SDL Error: %s\n",
                                                               SDL_GetError());

            err = EXIT_FAILURE;
        }
        else
        {
            //Initialize renderer color to blue
            if(EXIT_SUCCESS !=  SDL_SetRenderDrawColor(_sdlRenderer,
            										   0,
													   0,
													   255,
													   SDL_ALPHA_OPAQUE))
            {
                fprintf(stderr, "Error in, SDL_SetRenderDrawColor(), "
                                             "SDL Error: %s\n", SDL_GetError());

                err = EXIT_FAILURE;
            }
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _textureContainer.init(_sdlRenderer))
        {
            fprintf(stderr, "Error in _textureContainer.init()");

            err = EXIT_FAILURE;
        }
    }

    return err;
}

void Renderer::deinit()
{
    _textureContainer.deinit();

    if(_sdlRenderer) //sanity check
    {
        //Destroy window
        SDL_DestroyRenderer(_sdlRenderer);
        _sdlRenderer = nullptr;
    }

    if(_window) //sanity check
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

void Renderer::clearScreen()
{
    //clear screen
    if(EXIT_SUCCESS != SDL_RenderClear(_sdlRenderer))
    {
        fprintf(stderr, "Error in, SDL_RenderClear(), SDL Error: %s\n",
                                                               SDL_GetError());

        return;
    }
}

void Renderer::finishFrame()
{
    SDL_Rect renderQuad = { 0, 0, 0, 0 };

    SDL_Rect sourceQuad = { 0, 0, 0, 0 };

    const uint32_t USED_SIZE = static_cast<uint32_t>(_currWidgetCounter);

    //do the actual drawing of all stored images for THIS FRAME
    for(uint32_t i = 0; i < USED_SIZE; ++i)
    {
        renderQuad.x = _widgets[i].pos.x;
        renderQuad.y = _widgets[i].pos.y;

        sourceQuad = _textureContainer.getTextureFrameRect(_widgets[i].rsrcId,
                                                           _widgets[i].frame);

        renderQuad.w = sourceQuad.w;
        renderQuad.h = sourceQuad.h;

        if(EXIT_SUCCESS !=
                SDL_RenderCopy(_sdlRenderer,
                               _textureContainer.getTexture(_widgets[i].rsrcId),
                               &sourceQuad,
                               &renderQuad))
        {
            fprintf(stderr, "Error in, SDL_RenderCopy(), SDL Error: %s\n",
                                                               SDL_GetError());

            return;
        }
    }

    //reset widget counter
    _currWidgetCounter = 0;

    //------------- UPDATE SCREEN----------------
    SDL_RenderPresent(_sdlRenderer);
}




