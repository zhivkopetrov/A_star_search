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

  virtual void addAStarPathNode(const int32_t nodeX, const int32_t nodeY) = 0;

  virtual Point getNodeCoordinates(const int32_t nodeX,
                                   const int32_t nodeY) const = 0;
};

#endif /* GAMEENTITIES_PROXIES_GRIDCONTAINERPROXYINTERFACE_HPP_ */
