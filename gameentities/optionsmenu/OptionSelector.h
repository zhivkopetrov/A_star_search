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
#include "utils/EnumClassUtils.hpp"

//Forward declarations
class GameProxyInterface;
class InputEvent;

class OptionSelector : public OptionSelecterProxyInterface {
public:
  OptionSelector();
  virtual ~OptionSelector() = default;

  int32_t init(GameProxyInterface *gameInterface);

  void draw();

  void handleEvent(const InputEvent &e);

  void setOption(const Option option, const std::any &value);

private:
  virtual void onMenuButtonClicked(const MenuButtonType buttonType)
    override final;

  std::unordered_map<Option, std::any> _options;

  GameProxyInterface *_gameInterface;

  //TODO add animators
  std::array<MenuButton, getEnumClassValue(MenuButtonType::COUNT)> _buttons;
};

#endif /* GAMEENTITIES_OPTIONSMENU_OPTIONSELECTOR_H_ */
