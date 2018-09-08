/*
 * CommonStructs.h
 *
 *  Created on: Aug 26, 2018
 *      Author: zhivko
 */

#ifndef COMMON_COMMONSTRUCTS_HPP_
#define COMMON_COMMONSTRUCTS_HPP_

//C system headers

//C++ system headers
#include <cstdint>
#include <functional>

//Other libraries headers

//Own components headers

//Forward declarations


struct Point
{
	int32_t x;
	int32_t y;

	inline bool operator == (const Point& other) const
	{
		return (x == other.x && y == other.y);
	}

    inline bool operator != (const Point& other) const
    {
        return !(*this == other);
    }

	inline Point operator+ (const Point & other) const
	{
	    return { x + other.x, y + other.y };
	}
};

struct Node
{
	Node(const Point inputPosition, Node * inputParent = nullptr)
	{
	    parent   = inputParent;
	    position = inputPosition;
		G        = 0;
		H        = 0;
	}

	inline int32_t getScore() const
	{
		return G + H;
	}

	Node *  parent;
	int32_t G;
	int32_t H;
	Point   position;
};

#endif /* COMMON_COMMONSTRUCTS_HPP_ */
