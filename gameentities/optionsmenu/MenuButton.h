#ifndef GAMEENTITIES_OPTIONSMENU_MENUBUTTON_H_
#define GAMEENTITIES_OPTIONSMENU_MENUBUTTON_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/input/ButtonBase.h"
#include "gameentities/proxies/OptionSelecterProxyInterface.hpp"

//Forward declarations
class InputEvent;

class MenuButton : public ButtonBase {
public:
  MenuButton();
  virtual ~MenuButton() = default;

  int32_t init(OptionSelecterProxyInterface *interface,
               const MenuButtonType buttonType,
               const int32_t buttonRsrcId,
               const Point &startPos);

  virtual void handleEvent(const InputEvent &e) override final;

private:
  OptionSelecterProxyInterface *_menuInterface;

  MenuButtonType _buttonType;
};

#endif /* GAMEENTITIES_OPTIONSMENU_MENUBUTTON_H_ */
