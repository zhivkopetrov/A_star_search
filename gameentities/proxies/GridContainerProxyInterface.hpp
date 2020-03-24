#ifndef GAMEENTITIES_PROXIES_GRIDCONTAINERPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_GRIDCONTAINERPROXYINTERFACE_HPP_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/drawing/Point.h"

//Forward declarations

class GridContainerProxyInterface {
public:
  GridContainerProxyInterface() = default;
  virtual ~GridContainerProxyInterface() = default;

  virtual void addAStarPathNode(const Point& nodePos) = 0;

  virtual Point getNodeCoordinates(const Point& nodePos) const = 0;
};

#endif /* GAMEENTITIES_PROXIES_GRIDCONTAINERPROXYINTERFACE_HPP_ */
