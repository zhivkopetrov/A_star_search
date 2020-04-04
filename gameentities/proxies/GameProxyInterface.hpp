#ifndef GAMEENTITIES_PROXIES_GAMEPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_GAMEPROXYINTERFACE_HPP_

//C system headers

//C++ system headers
#include <cstdint>
#include <string>
#include <any>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "utils/drawing/Point.h"

//Forward declarations



class GameProxyInterface {
public:
  GameProxyInterface() = default;
  virtual ~GameProxyInterface() = default;

  virtual void onNodeChanged(const NodeType nodeType, const Point &nodePos) = 0;

  virtual void onAnimFinished(const AnimType animType) = 0;

  virtual void onOptionChanged(const Option option,
                               const std::any &value) = 0;

  virtual void onOptionAnimStatusChange(const OptionAnimStatus type) = 0;
};

#endif /* GAMEENTITIES_PROXIES_GAMEPROXYINTERFACE_HPP_ */
