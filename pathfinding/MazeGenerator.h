#ifndef PATHFINDING_MAZEGENERATOR_H_
#define PATHFINDING_MAZEGENERATOR_H_

//C system headers

//C++ system headers
#include <vector>

//Other libraries headers

//Own components headers
#include "AStar.h"
#include "utils/drawing/Point.h"

//Forward declarations

class MazeGenerator {
public:
  MazeGenerator();
  ~MazeGenerator() = default;

  int32_t init(const int32_t mazeWidth, const int32_t mazeHeight,
               const bool isDiagonalMovementEnabled,
               HeuristicFunction heuristic);

  void removeCollision(const Point &position);

  inline void setStartNodePos(const Point &startNodePos) {
    _startNodePos = startNodePos;
  }

  inline void setEndNodePos(const Point &endNodePos) {
    _endNodePos = endNodePos;
  }

  inline Point getEndNodePos() const {
    return _endNodePos;
  }

  inline void addCollision(const Point &position) {
    _walls.emplace_back(position);
  }

  inline bool isReadyToEvaluate() const {
    return (_startNodePos != Point::UNDEFINED) &&
                                            (_endNodePos != Point::UNDEFINED);
  }

  inline std::vector<Point> findPath() {
    return _aStar.findPath(_startNodePos, _endNodePos);
  }

  inline void clearCollisions() {
    _walls.clear();
  }

private:
  AStar _aStar;

  Point _startNodePos;
  Point _endNodePos;

  std::vector<Point> _walls;
};

#endif /* PATHFINDING_MAZEGENERATOR_H_ */
