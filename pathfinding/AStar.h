/*
 * AStar.h
 *
 *  Created on: Aug 26, 2018
 *      Author: zhivko
 */

#ifndef PATHFINDING_ASTAR_H_
#define PATHFINDING_ASTAR_H_

//C system headers

//C++ system headers
#include <set>
#include <vector>

//Other libraries headers

//Own components headers
#include "common/CommonStructs.hpp"

#include "pathfinding/Heuristic.h"

//Forward declarations


class AStar
{
	public:
		AStar();

		~AStar() = default;

		int32_t init(const int32_t              mazeWidth,
				     const int32_t              mazeHeight,
				     const bool                 isDiagonalMovementEnabled,
					 HeuristicFunction          heuristic,
					 const std::vector<Point> * walls);

		std::vector<Point> findPath(const Point & source, const Point & target);

	private:
		bool detectCollision(const Point & position);

		Node* findNodeOnList(const std::set<Node*> & nodes,
							 const Point &           position);

		void releaseNodes(std::set<Node*> & nodes);

		HeuristicFunction          _heuristic;
		std::vector<Point>         _moveDirections;
		const std::vector<Point> * _walls;

		uint32_t                   _allowedDirectionsCount;
		int32_t			           _mazeWidth;
		int32_t			           _mazeHeight;
};


#endif /* PATHFINDING_ASTAR_H_ */
