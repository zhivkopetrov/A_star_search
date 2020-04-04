#ifndef GAMEENTITIES_OPTIONSMENU_OPTIONSELECTOR_H_
#define GAMEENTITIES_OPTIONSMENU_OPTIONSELECTOR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <unordered_map>
#include <any>
#include <array>
#include <vector>

//Other libraries headers

//Own components headers
#include "MenuButton.h"
#include "gameentities/proxies/OptionSelecterProxyInterface.hpp"
#include "common/CommonDefines.h"
#include "utils/drawing/Image.h"
#include "utils/drawing/Text.h"

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

  std::vector<const Widget *> getWidgetsState() const;

  void onMoveAnimFinished(const OptionAnimStatus type);

  void setTextLevelId(const int32_t levelId);

private:
  virtual void onMenuButtonClicked(const MenuButtonType buttonType)
    override final;

  void activateMenu();
  void deactivateMenu();

  void centerLevelValueText();

  enum MenuButtonDefines {
    TOGGLE_MENU_IDX,
    INCR_LEVEL_IDX,
    DECR_LEVEL_IDX,
    ALLOW_DIAGONAL_IDX,
    FORBID_DIAGONAL_IDX,

    BUTTONS_COUNT
  };

  enum TextDefines {
    DIAGONAL_TEXT_IDX,
    LEVEL_TEXT_IDX,
    LEVEL_NUMBER_TEXT_IDX,

    TEXTS_COUNT
  };

  std::unordered_map<Option, std::any> _options;

  GameProxyInterface *_gameInterface;

  std::array<MenuButton, BUTTONS_COUNT> _buttons;
  Image _menuImg;

  std::array<Text, TEXTS_COUNT> _texts;

  bool _isMenuActive;
  bool _isBeingAnimated;
};

#endif /* GAMEENTITIES_OPTIONSMENU_OPTIONSELECTOR_H_ */
