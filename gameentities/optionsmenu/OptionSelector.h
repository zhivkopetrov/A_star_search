#ifndef GAMEENTITIES_OPTIONSMENU_OPTIONSELECTOR_H_
#define GAMEENTITIES_OPTIONSMENU_OPTIONSELECTOR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <unordered_map>
#include <any>
#include <array>

//Other libraries headers

//Own components headers
#include "MenuButton.h"
#include "gameentities/proxies/OptionSelecterProxyInterface.hpp"
#include "common/CommonDefines.h"
#include "utils/drawing/Image.h"


//Forward declarations
class GameProxyInterface;
class InputEvent;

class OptionSelector : public OptionSelecterProxyInterface {
public:
  OptionSelector();
  virtual ~OptionSelector() = default;

  int32_t init(GameProxyInterface *gameInterface);

  void draw();

  /** returns bool - contains event */
  bool handleEvent(const InputEvent &e);

  void setOption(const Option option, const std::any &value);

private:
  virtual void onMenuButtonClicked(const MenuButtonType buttonType)
    override final;

  void activateMenu();
  void deactivateMenu();

  enum InternalDefines {
    TOGGLE_MENU_IDX = 0,
    ALLOW_DIAGONAL_IDX = 3,
    FORBID_DIAGONAL_IDX = 4,
    BUTTONS_COUNT = 5,

    MENU_X = 1510,
    MENY_Y = 25,
    MENU_OFFSET_X = 325
  };

  std::unordered_map<Option, std::any> _options;

  GameProxyInterface *_gameInterface;

  //TODO add animators
  std::array<MenuButton, BUTTONS_COUNT> _buttons;
  Image _menuImg;

  bool _isMenuActive;
};

#endif /* GAMEENTITIES_OPTIONSMENU_OPTIONSELECTOR_H_ */
