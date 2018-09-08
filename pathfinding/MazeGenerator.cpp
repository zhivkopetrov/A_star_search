/*
 * MazeGenerator.h
 *
 *  Created on: Aug 26, 2018
 *      Author: zhivko
 */

//Corresponding header
#include "MazeGenerator.h"

//C system headers

//C++ system headers
#include <algorithm>
#include <cstdio>

//Other libraries headers

//Own components headers


MazeGenerator::MazeGenerator()
{

}

int32_t MazeGenerator::init(const int32_t     mazeWidth,
						    const int32_t     mazeHeight,
						    const bool        isDiagonalMovementEnabled,
							HeuristicFunction heuristic)
{
	int32_t err = EXIT_SUCCESS;

	_startNodePos = _POS_UNDEFINED;
	_endNodePos = _POS_UNDEFINED;

	if(EXIT_SUCCESS != _aStar.init(mazeWidth,
								   mazeHeight,
								   isDiagonalMovementEnabled,
								   heuristic,
								   &_walls))
	{
		fprintf(stderr, "Error, _aStar.init() failed\n");

		err = EXIT_FAILURE;
	}

	return err;
}

void MazeGenerator::removeCollision(const Point & position)
{
    auto it = std::find(_walls.begin(), _walls.end(), position);
    if (it != _walls.end())
    {
        _walls.erase(it);
    }
}

void MazeGenerator::clearCollisions()
{
    _walls.clear();
}

std::vector<Point> MazeGenerator::findPath()
{
    if((_startNodePos != _POS_UNDEFINED) && (_endNodePos != _POS_UNDEFINED))
    {
        return _aStar.findPath(_startNodePos, _endNodePos);
    }

    return std::vector<Point>();
}



