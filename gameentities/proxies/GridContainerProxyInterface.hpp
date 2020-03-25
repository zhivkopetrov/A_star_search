#ifndef GAMEENTITIES_PROXIES_GRIDCONTAINERPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_GRIDCONTAINERPROXYINTERFACE_HPP_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "utils/drawing/Point.h"

//Forward declarations

struct Obstacle {
  explicit Obstacle(const int32_t row, const int32_t col,
                    const uint8_t inputFrameId)
      : pos(row, col), frameId(inputFrameId) {
  }
  const Point pos;
  const uint8_t frameId;
};

class GridContainerProxyInterface {
public:
  GridContainerProxyInterface() = default;
  virtual ~GridContainerProxyInterface() = default;

  virtual void addPathNode(const Point &nodePos) = 0;

  virtual Point getNodeCoordinates(const Point &nodePos) const = 0;

  virtual void removePredefinedObstacles(
      const std::vector<Obstacle> &obstacles) = 0;
  virtual void placePredefinedObstacles(
      const std::vector<Obstacle> &obstacles) = 0;
};

#endif /* GAMEENTITIES_PROXIES_GRIDCONTAINERPROXYINTERFACE_HPP_ */
