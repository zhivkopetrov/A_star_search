#ifndef GAMEENTITIES_PROXIES_OBSTACLEHANDLERPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_OBSTACLEHANDLERPROXYINTERFACE_HPP_

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "utils/drawing/Point.h"

//Forward declarations

class ObstacleHandlerProxyInterface {
public:
  ObstacleHandlerProxyInterface() = default;
  virtual ~ObstacleHandlerProxyInterface() = default;

  virtual bool isIntersectingObstacle(const Point &nodePos) const = 0;
};

#endif /* GAMEENTITIES_PROXIES_OBSTACLEHANDLERPROXYINTERFACE_HPP_ */
