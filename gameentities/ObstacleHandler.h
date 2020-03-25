#ifndef GAMEENTITIES_OBSTACLEHANDLER_H_
#define GAMEENTITIES_OBSTACLEHANDLER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "proxies/ObstacleHandlerProxyInterface.hpp"

//Forward declarations

class ObstacleHandler: public ObstacleHandlerProxyInterface {
public:
  ObstacleHandler() = default;
  virtual ~ObstacleHandler() = default;

  int32_t init();

  inline void addObstacle(const Point &nodePos) {
    _addedObstacles.push_back(nodePos);
  }

  inline void clearAddedObstacles() {
    _addedObstacles.clear();
  }

  void removeObstacle(const Point &nodePos);

  virtual const std::vector<Point>& getPredefinedObstacles() const
      override final;

private:
  virtual bool isIntersectingObstacle(const Point &nodePos) const
      override final;

  std::vector<Point> _predefinedObstacles;
  std::vector<Point> _addedObstacles;
};

#endif /* GAMEENTITIES_OBSTACLEHANDLER_H_ */
