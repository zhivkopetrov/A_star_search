//Corresponding header
#include "ObstacleHandler.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <algorithm>

//Other libraries headers

//Own components headers

int32_t ObstacleHandler::init() {
  return EXIT_SUCCESS;
}

void ObstacleHandler::removeObstacle(const Point &nodePos) {
  auto it = std::find(_addedObstacles.begin(), _addedObstacles.end(), nodePos);
  if (it != _addedObstacles.end()) {
    _addedObstacles.erase(it);
  }
}

const std::vector<Point>& ObstacleHandler::getPredefinedObstacles() const {
  return _predefinedObstacles;
}

bool ObstacleHandler::isIntersectingObstacle(const Point &nodePos) const {
  const bool isIntersectingPredefined = std::find(_predefinedObstacles.begin(),
                                            _predefinedObstacles.end(), nodePos)
                                        != _predefinedObstacles.end();
  const bool isIntersectingAdded = std::find(_addedObstacles.begin(),
                                       _addedObstacles.end(), nodePos)
                                   != _addedObstacles.end();

  return isIntersectingPredefined || isIntersectingAdded;
}
