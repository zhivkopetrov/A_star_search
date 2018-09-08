/*
 * SDLLoader.h
 *
 *  Created on: 6 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef SDL_SDLLOADER_H_
#define SDL_SDLLOADER_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations


class SDLLoader
{
    public:
        //forbid the default constructor and destructor
        SDLLoader() = delete;
        virtual ~SDLLoader() = delete;

        /** @brief used to initialise external SDL sub-systems
         *
         *  @returns int32_t - error code
         * */
        static int32_t init();

        /** @brief used to deinitialse all external SDL sub-systems
         * */
        static void deinit();
};

#endif /* SDL_SDLLOADER_H_ */
