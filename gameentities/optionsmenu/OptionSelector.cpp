//Corresponding header
#include "OptionSelector.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "gameentities/proxies/GameProxyInterface.hpp"
#include "sdl/InputEvent.h"
#include "utils/EnumClassUtils.hpp"
#include "utils/Log.h"

OptionSelector::OptionSelector()
    : _gameInterface(nullptr), _isMenuActive(false) {

}

int32_t OptionSelector::init(GameProxyInterface *gameInterface) {
  _gameInterface = gameInterface;

  _menuImg.create(Textures::MENU);
  _menuImg.setPosition(MENU_X, MENY_Y);

  const MenuButtonCfg buttonCfgs[BUTTONS_COUNT] {
    { this, Point(MENU_X, MENY_Y), Textures::MENU_SETTINGS, MenuButtonType::TOGGLE_MENU },
    { this, Point(MENU_X + 200, MENY_Y + 114), Textures::MENU_PLUS, MenuButtonType::INCREASE },
    { this, Point(MENU_X + 320, MENY_Y + 114), Textures::MENU_MINUS, MenuButtonType::DECREASE },
    { this, Point(MENU_X + 320, MENY_Y + 20), Textures::MENU_CHECK,  MenuButtonType::ALLOW_DIAGONAL },
    { this, Point(MENU_X + 320, MENY_Y + 20), Textures::MENU_CROSS,  MenuButtonType::FORBID_DIAGONAL } };

  for (int32_t i = 0; i < BUTTONS_COUNT; ++i) {
    if (_buttons[i].init(buttonCfgs[i])) {
      LOGERR("Error, _buttons[%d].init() failed", i);
      return EXIT_FAILURE;
    }
  }

  deactivateMenu();
  _buttons[ALLOW_DIAGONAL_IDX].lockInput();
  _buttons[ALLOW_DIAGONAL_IDX].hide();

  return EXIT_SUCCESS;
}

void OptionSelector::draw() {
  if (_isMenuActive) {
    _menuImg.draw();
    for (auto &button : _buttons) {
      button.draw();
    }
  } else {
    _menuImg.draw();
    _buttons[TOGGLE_MENU_IDX].draw();
  }
}

bool OptionSelector::handleEvent(const InputEvent &e) {
  bool containsEvent = false;
  if (_isMenuActive) {
    for (auto &button : _buttons) {
      if (button.isInputUnlocked() && button.containsEvent(e)) {
        button.handleEvent(e);
        containsEvent = true;
        break;
      }
    }
  } else {
    if (_buttons[TOGGLE_MENU_IDX].isInputUnlocked() &&
        _buttons[TOGGLE_MENU_IDX].containsEvent(e)) {
      _buttons[TOGGLE_MENU_IDX].handleEvent(e);
      containsEvent = true;
    }
  }

  if (!containsEvent) {
    containsEvent = _menuImg.containsPoint(e.pos);
  }

  return containsEvent;
}

void OptionSelector::setOption(const Option option, const std::any &value) {
  _options[option] = value;
  _gameInterface->onOptionChanged(option, value);
}

void OptionSelector::onMenuButtonClicked(const MenuButtonType buttonType) {
  switch (buttonType) {
  case MenuButtonType::TOGGLE_MENU:
    if (_isMenuActive) {
      deactivateMenu();
    } else {
      activateMenu();
    }
    break;
  case MenuButtonType::INCREASE:
    setOption(Option::LEVEL_CHANGE, 1);
    break;

  case MenuButtonType::DECREASE:
    setOption(Option::LEVEL_CHANGE, -1);
    break;

  case MenuButtonType::ALLOW_DIAGONAL:
    _buttons[ALLOW_DIAGONAL_IDX].lockInput();
    _buttons[ALLOW_DIAGONAL_IDX].hide();
    _buttons[FORBID_DIAGONAL_IDX].unlockInput();
    _buttons[FORBID_DIAGONAL_IDX].show();
    setOption(Option::DIAGONAL_MOVEMENT, true);
    break;

  case MenuButtonType::FORBID_DIAGONAL:
    _buttons[ALLOW_DIAGONAL_IDX].unlockInput();
    _buttons[ALLOW_DIAGONAL_IDX].show();
    _buttons[FORBID_DIAGONAL_IDX].lockInput();
    _buttons[FORBID_DIAGONAL_IDX].hide();
    setOption(Option::DIAGONAL_MOVEMENT, false);
    break;

  default:
    LOGERR("Received unknown MenuButtonType Id: %hhu",
        getEnumClassValue(buttonType))
    ;
  }
}

void OptionSelector::activateMenu() {
  _isMenuActive = true;
  _buttons[TOGGLE_MENU_IDX].moveLeft(MENU_OFFSET_X);

  _menuImg.moveLeft(MENU_OFFSET_X);
}

void OptionSelector::deactivateMenu() {
  _isMenuActive = false;
  _buttons[TOGGLE_MENU_IDX].moveRight(MENU_OFFSET_X);

  _menuImg.moveRight(MENU_OFFSET_X);
}

