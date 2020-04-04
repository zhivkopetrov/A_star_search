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
    : _gameInterface(nullptr), _isMenuActive(false), _isBeingAnimated(false) {

}

int32_t OptionSelector::init(GameProxyInterface *gameInterface) {
  _gameInterface = gameInterface;

  _menuImg.create(Textures::MENU);
  _menuImg.setPosition(OptionMenuDimensions::MENU_X,
      OptionMenuDimensions::MENU_Y);

  const MenuButtonCfg buttonCfgs[BUTTONS_COUNT] { { this, Point(
      OptionMenuDimensions::MENU_X, OptionMenuDimensions::MENU_Y),
      Textures::MENU_SETTINGS, MenuButtonType::TOGGLE_MENU }, { this, Point(
      OptionMenuDimensions::MENU_X + 200, OptionMenuDimensions::MENU_Y + 114),
      Textures::MENU_PLUS, MenuButtonType::INCREASE }, { this, Point(
      OptionMenuDimensions::MENU_X + 320, OptionMenuDimensions::MENU_Y + 114),
      Textures::MENU_MINUS, MenuButtonType::DECREASE }, { this, Point(
      OptionMenuDimensions::MENU_X + 320, OptionMenuDimensions::MENU_Y + 20),
      Textures::MENU_CHECK, MenuButtonType::ALLOW_DIAGONAL }, { this, Point(
      OptionMenuDimensions::MENU_X + 320, OptionMenuDimensions::MENU_Y + 20),
      Textures::MENU_CROSS, MenuButtonType::FORBID_DIAGONAL } };

  for (int32_t i = 0; i < BUTTONS_COUNT; ++i) {
    if (_buttons[i].init(buttonCfgs[i])) {
      LOGERR("Error, _buttons[%d].init() failed", i);
      return EXIT_FAILURE;
    }
  }

  _buttons[TOGGLE_MENU_IDX].moveRight(OptionMenuDimensions::MENU_OFFSET_X);
  _menuImg.moveRight(OptionMenuDimensions::MENU_OFFSET_X);
  _buttons[ALLOW_DIAGONAL_IDX].lockInput();
  _buttons[ALLOW_DIAGONAL_IDX].hide();

  return EXIT_SUCCESS;
}

void OptionSelector::draw() {
  if (_isBeingAnimated) {
    return;
  }

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

std::vector<const Widget*> OptionSelector::getWidgetsState() const {
  std::vector<const Widget*> widgets;
  widgets.reserve(BUTTONS_COUNT + 1);
  widgets.push_back(&_menuImg);
  for (const auto &button : _buttons) {
    widgets.push_back(& (button.getButtonImage()));
  }

  return widgets;
}

void OptionSelector::onMoveAnimFinished(const OptionAnimStatus type) {
  _isBeingAnimated = false;

  if (OptionAnimStatus::END_OPEN_ANIM == type) {
    _isMenuActive = true;
  } else if (OptionAnimStatus::END_CLOSE_ANIM == type) {
    _isMenuActive = false;
  } else {
    LOGERR("Error, received unknown OptionAnimType: %hhu",
        getEnumClassValue(type));
  }
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
  _isBeingAnimated = true;

  //first move the button and menu background image so they can be "baked" into
  //the FBO
  _buttons[TOGGLE_MENU_IDX].moveLeft(OptionMenuDimensions::MENU_OFFSET_X);
  _menuImg.moveLeft(OptionMenuDimensions::MENU_OFFSET_X);

  _gameInterface->onOptionAnimStatusChange(
      OptionAnimStatus::UPDATE_ANIM_CONTENT);
  _gameInterface->onOptionAnimStatusChange(OptionAnimStatus::START_OPEN_ANIM);
}

void OptionSelector::deactivateMenu() {
  _isBeingAnimated = true;

  //first "bake" the button and menu background image into the FBO
  _gameInterface->onOptionAnimStatusChange(
      OptionAnimStatus::UPDATE_ANIM_CONTENT);
  _gameInterface->onOptionAnimStatusChange(OptionAnimStatus::START_CLOSE_ANIM);

  //the move them
  _buttons[TOGGLE_MENU_IDX].moveRight(OptionMenuDimensions::MENU_OFFSET_X);
  _menuImg.moveRight(OptionMenuDimensions::MENU_OFFSET_X);
}

