/*
 * Heuristic.cpp
 *
 *  Created on: Aug 26, 2018
 *      Author: zhivko
 */

#include "Heuristic.h"
//Corresponding header

//C system headers

//C++ system headers
#include <cmath>

//Other libraries headers

//Own components headers
#include "common/CommonStructs.hpp"


Point Heuristic::getDelta(const Point & source, const Point & target)
{
    return { abs(source.x - target.x),  abs(source.y - target.y) };
}

int32_t Heuristic::manhattan(const Point & source, const Point & target)
{
    const Point delta = getDelta(source, target);
    return (10 * (delta.x + delta.y));
}

int32_t Heuristic::diagonal(const Point & source, const Point & target)
{
	const Point delta = getDelta(source, target);
    return (10 * (delta.x + delta.y)) + ((-6) * std::min(delta.x, delta.y));
}

int32_t Heuristic::euclidean(const Point & source, const Point & target)
{
	const Point delta = getDelta(source, target);
    return static_cast<int32_t>(
    				  (10 * sqrt((delta.x * delta.x) + (delta.y * delta.y))));
}





