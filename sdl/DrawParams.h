/*
 * DrawParams.h
 *
 *  Created on: 6 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef SDL_DRAWPARAMS_H_
#define SDL_DRAWPARAMS_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers
#include <SDL2/SDL_rect.h>

//Own components headers


struct DrawParams
{
    DrawParams()
    {
        reset();
    }

    void reset()
    {
        pos    = { INT32_MAX, INT32_MAX };

        rsrcId = UINT8_MAX;

        frame  = UINT8_MAX;
    }

    //Top left position of texture
    SDL_Point pos;

    //unique resource ID of the texture
    uint8_t   rsrcId;

    //frame index for the texture
    uint8_t   frame;
};

#endif /* SDL_DRAWPARAMS_H_ */

