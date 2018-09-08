/*
 * Text.cpp
 *
 *  Created on: 9 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "Text.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers

//Own components headers
#include "sdl/TextureContainer.h"


Text::Text() : isActive(false),
               _textureContainer(nullptr),
               _fontSize(0),
               _width(0),
               _height(0)
{

}

int32_t Text::init(TextureContainer *       textureContainer,
                   const uint8_t            rsrcId,
                   const SDL_Point          startPoint,
                   const char *             startText,
                   const int32_t            fontSize)
{
    _textureContainer  = textureContainer;
    drawParams.rsrcId = rsrcId;
    drawParams.frame  = 0;
    drawParams.pos    = startPoint;
    _fontSize          = fontSize;

    _textureContainer->setText(startText,
                               fontSize,
                               rsrcId,
                               &_width,
                               &_height);

    return EXIT_SUCCESS;
}

void Text::setText(const char * text)
{
    _textureContainer->setText(text,
                               _fontSize,
                               drawParams.rsrcId,
                               &_width,
                               &_height);
}





