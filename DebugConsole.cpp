/*
 * DebugConsole.cpp
 *
 *  Created on: 9 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "DebugConsole.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <string>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "common/CommonDefines.h"
#include "common/Time.h"

#define UPDATE_SKIPS 500


DebugConsole::DebugConsole() : _totalElapsedTime(0),
                               _totalUpdates(0),
                               _updateCounter(UPDATE_SKIPS),
                               _isActive(false)
{

}

int32_t DebugConsole::init(TextureContainer * textureContainer)
{
    return _fpsText.init(textureContainer,
                         Textures::FPS_TEXT,
                         SDL_Point { 20, 20 },
                         "0",
                         FontSize::SMALL);
}

void DebugConsole::handleEvent(SDL_Event & e)
{
    if(SDL_KEYUP != e.type)
    {
        return;
    }

    if(SDLK_BACKQUOTE == e.key.keysym.sym)
    {
        _isActive = !_isActive;
    }
}

//update fps text once in a while to get a stable(not constantly changing) image
void DebugConsole::update(const int32_t elapsedTime)
{
    _totalElapsedTime += elapsedTime;
    ++_totalUpdates;

    --_updateCounter;

    if(0 < _updateCounter)
    {
        return;
    }

    _updateCounter = UPDATE_SKIPS;

    _fpsText.setText(std::to_string(
                    MICROSECOND / (_totalElapsedTime / _totalUpdates)).c_str());
}




