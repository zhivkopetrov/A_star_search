//Corresponding header
#include "MazeGenerator.h"

//C system headers

//C++ system headers
#include <algorithm>

//Other libraries headers

//Own components headers
#include "utils/Log.h"

int32_t MazeGenerator::init(const int32_t mazeWidth, const int32_t mazeHeight,
                            const bool isDiagonalMovementEnabled,
                            HeuristicFunction heuristic) {
  int32_t err = EXIT_SUCCESS;

  _startNodePos = Point::UNDEFINED;
  _endNodePos = Point::UNDEFINED;

  if (EXIT_SUCCESS != _aStar.init(mazeWidth, mazeHeight,
          isDiagonalMovementEnabled, heuristic, &_walls)) {
    LOGERR("Error, _aStar.init() failed");

    err = EXIT_FAILURE;
  }

  return err;
}

void MazeGenerator::removeCollision(const Point &position) {
  auto it = std::find(_walls.begin(), _walls.end(), position);
  if (it != _walls.end()) {
    _walls.erase(it);
  }
}

void MazeGenerator::clearCollisions() {
  _walls.clear();
}

std::vector<Point> MazeGenerator::findPath() {
  if ( (_startNodePos != Point::UNDEFINED) &&
      (_endNodePos != Point::UNDEFINED)) {
    return _aStar.findPath(_startNodePos, _endNodePos);
  }

  return std::vector<Point>();
}

