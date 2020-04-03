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

struct MenuButtonCfg {
  MenuButtonCfg(OptionSelecterProxyInterface *inputInterface,
                         const Point &inputStartPos,
                         const int32_t inputButtonRsrcId,
                         const MenuButtonType inputButtonType)
      : interface(inputInterface), startPos(inputStartPos),
        buttonRsrcId(inputButtonRsrcId), buttonType(inputButtonType) {

  }

  MenuButtonCfg() = delete;

  OptionSelecterProxyInterface *interface;
  const Point startPos;
  const int32_t buttonRsrcId;
  const MenuButtonType buttonType;
};

class MenuButton: public ButtonBase {
public:
  MenuButton();
  virtual ~MenuButton() = default;

  int32_t init(const MenuButtonCfg &cfg);

  virtual void handleEvent(const InputEvent &e) override final;

  virtual void lockInput() override final;

  virtual void unlockInput() override final;

private:
  OptionSelecterProxyInterface *_menuInterface;

  MenuButtonType _buttonType;
};

#endif /* GAMEENTITIES_OPTIONSMENU_MENUBUTTON_H_ */
