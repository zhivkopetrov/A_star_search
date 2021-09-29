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

  OptionSelecterProxyInterface *interface;
  const Point startPos;
  const int32_t buttonRsrcId;
  const MenuButtonType buttonType;
};

class MenuButton: public ButtonBase {
public:
  int32_t init(const MenuButtonCfg &cfg);

  void handleEvent(const InputEvent &e) final;

  void lockInput() final;

  void unlockInput() final;

private:
  OptionSelecterProxyInterface *_menuInterface = nullptr;

  MenuButtonType _buttonType = MenuButtonType::UNKNOWN;
};

#endif /* GAMEENTITIES_OPTIONSMENU_MENUBUTTON_H_ */
