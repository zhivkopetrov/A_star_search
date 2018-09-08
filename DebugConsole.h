/*
 * DebugConsole.h
 *
 *  Created on: 9 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef DEBUGCONSOLE_H_
#define DEBUGCONSOLE_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "gameentities/Text.h"

//Forward declarations
class TextureContainer;
union SDL_Event;


class DebugConsole
{
    public:
        DebugConsole();
        virtual ~DebugConsole() = default;

        int32_t init(TextureContainer * textureContainer);

        /** @ brief used to toggle active/inactive status of the debug console
         * */
        void handleEvent(SDL_Event & e);

        void update(const int32_t elapsedTime);

        inline DrawParams * getDrawParams()
        {
            return &_fpsText.drawParams;
        }

        inline bool isActive() const
        {
            return _isActive;
        }

    private:
        Text    _fpsText;

        int32_t _totalElapsedTime;

        int32_t _totalUpdates;

        int32_t _updateCounter;

        bool    _isActive;
};

#endif /* DEBUGCONSOLE_H_ */


