#ifndef GAMEENTITIES_PROXIES_GAMEPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_GAMEPROXYINTERFACE_HPP_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/drawing/Point.h"

//Forward declarations

enum class NodeType {
  WALL_ADD = 0, WALL_REMOVE, START_CHANGE, END_CHANGE
};

class GameProxyInterface {
public:
  GameProxyInterface() = default;
  virtual ~GameProxyInterface() = default;

  virtual void onNodeChanged(const NodeType nodeType, const Point &nodePos) = 0;

  virtual void onAnimFinished() = 0;
};

#endif /* GAMEENTITIES_PROXIES_GAMEPROXYINTERFACE_HPP_ */
