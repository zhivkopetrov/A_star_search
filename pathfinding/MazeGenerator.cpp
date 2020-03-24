//Corresponding header
#include "MazeGenerator.h"

//C system headers

//C++ system headers
#include <algorithm>

//Other libraries headers

//Own components headers
#include "utils/Log.h"

MazeGenerator::MazeGenerator()
    : _startNodePos(Point::UNDEFINED), _endNodePos(Point::UNDEFINED) {

}

int32_t MazeGenerator::init(const int32_t mazeWidth, const int32_t mazeHeight,
                            const bool isDiagonalMovementEnabled,
                            HeuristicFunction heuristic) {
  int32_t err = EXIT_SUCCESS;

  if (EXIT_SUCCESS != _aStar.init(mazeWidth, mazeHeight,
          isDiagonalMovementEnabled, heuristic, &_walls)) {
    LOGERR("Error, _aStar.init() failed");

    err = EXIT_FAILURE;
  }

  return err;
}

void MazeGenerator::setStartNodePos(const Point &startNodePos) {
  _startNodePos = startNodePos;
  if (_startNodePos == _endNodePos) {
    _endNodePos = Point::UNDEFINED;
  }
}

void MazeGenerator::setEndNodePos(const Point &endNodePos) {
  _endNodePos = endNodePos;
  if (_endNodePos == _startNodePos) {
    _startNodePos = Point::UNDEFINED;
  }
}

void MazeGenerator::addCollision(const Point &position) {
  _walls.emplace_back(position);

  if (_startNodePos == position) {
    _startNodePos = Point::UNDEFINED;
  }
  if (_endNodePos == position) {
    _endNodePos = Point::UNDEFINED;
  }
}

void MazeGenerator::removeNode(const Point &position) {
  if (_startNodePos == position) {
    _startNodePos = Point::UNDEFINED;
    return;
  }
  if (_endNodePos == position) {
    _endNodePos = Point::UNDEFINED;
    return;
  }
  auto it = std::find(_walls.begin(), _walls.end(), position);
  if (it != _walls.end()) {
    _walls.erase(it);
  }
}

void MazeGenerator::clear() {
  _startNodePos = Point::UNDEFINED;
  _endNodePos = Point::UNDEFINED;
}

