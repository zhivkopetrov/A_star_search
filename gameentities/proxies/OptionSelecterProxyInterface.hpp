#ifndef GAMEENTITIES_PROXIES_OBPTIONSSELECTORRPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_OBPTIONSSELECTORRPROXYINTERFACE_HPP_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations

enum class MenuButtonType : uint8_t {
  UNKNOWN,
  OPEN_MENU,
  CLOSE_MENU,
  INCREASE,
  DECREASE,
  ENABLE_DIAGONAL,
  DISABLE_DIAGONAL,

  COUNT
};

class OptionSelecterProxyInterface {
public:
  OptionSelecterProxyInterface() = default;
  virtual ~OptionSelecterProxyInterface() = default;

  virtual void onMenuButtonClicked(const MenuButtonType buttonType) = 0;
};

#endif /* GAMEENTITIES_PROXIES_OBPTIONSSELECTORRPROXYINTERFACE_HPP_ */
