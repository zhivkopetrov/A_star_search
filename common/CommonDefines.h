/*
 * CommonDefines.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef COMMON_COMMONDEFINES_H_
#define COMMON_COMMONDEFINES_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations


namespace Textures
{
    enum : uint8_t
    {
        VERTICAL_LINE,
		HORIZONTAL_LINE,
		START_NODE,
		END_NODE,
		WALL,
		A_STAR_PATH,
        FPS_TEXT,
        NO_AVAILABLE_PATH_TEXT,

        COUNT
    };
}

namespace Direction
{
    enum : uint8_t
    {
        DOWN,
        LEFT,
        RIGHT,
        UP,

        COUNT
    };
}

namespace FontSize
{
    enum
    {
        SMALL,
        BIG
    };
}

#endif /* COMMON_COMMONDEFINES_H_ */


