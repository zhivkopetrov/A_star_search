/*
 * GameProxyInterface.hpp
 *
 *  Created on: Aug 27, 2018
 *      Author: zhivko
 */

#ifndef GAMEPROXYINTERFACE_HPP_
#define GAMEPROXYINTERFACE_HPP_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations

enum class NodeType
{
    WALL_ADD = 0,
    WALL_REMOVE,
    START_CHANGE,
    END_CHANGE
};


class GameProxyInterface
{
    public:
        GameProxyInterface() = default;
        virtual ~GameProxyInterface() = default;

        virtual void onNodeChanged(const NodeType nodeType,
                                   const int32_t  nodeX,
                                   const int32_t  nodeY) = 0;
};

#endif /* GAMEPROXYINTERFACE_HPP_ */
