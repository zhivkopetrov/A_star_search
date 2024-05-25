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
  _menuImg.setPosition(
      OptionMenuDimensions::MENU_X + OptionMenuDimensions::MENU_OFFSET_X,
      OptionMenuDimensions::MENU_Y);
  _menuImg.activateAlphaModulation();
  _menuImg.setOpacity(OptionMenuDimensions::CLOSED_MENU_OPACITY);

  const MenuButtonCfg buttonCfgs[BUTTONS_COUNT] { { this, Point(
      OptionMenuDimensions::MENU_X, OptionMenuDimensions::MENU_Y),
      Textures::MENU_SETTINGS, MenuButtonType::TOGGLE_MENU }, { this, Point(
      OptionMenuDimensions::MENU_X + 195, OptionMenuDimensions::MENU_Y + 124),
      Textures::MENU_PLUS, MenuButtonType::INCREASE }, { this, Point(
      OptionMenuDimensions::MENU_X + 320, OptionMenuDimensions::MENU_Y + 124),
      Textures::MENU_MINUS, MenuButtonType::DECREASE }, { this, Point(
      OptionMenuDimensions::MENU_X + 320, OptionMenuDimensions::MENU_Y + 40),
      Textures::MENU_CROSS, MenuButtonType::ALLOW_DIAGONAL }, { this, Point(
      OptionMenuDimensions::MENU_X + 320, OptionMenuDimensions::MENU_Y + 40),
      Textures::MENU_CHECK, MenuButtonType::FORBID_DIAGONAL } };

  for (int32_t i = 0; i < BUTTONS_COUNT; ++i) {
    if (_buttons[i].init(buttonCfgs[i])) {
      LOGERR("Error, _buttons[%d].init() failed", i);
      return EXIT_FAILURE;
    }
  }

  _buttons[TOGGLE_MENU_IDX].activateAlphaModulation();
  _buttons[TOGGLE_MENU_IDX].moveRight(OptionMenuDimensions::MENU_OFFSET_X);
  _buttons[TOGGLE_MENU_IDX].setOpacity(
      OptionMenuDimensions::CLOSED_MENU_OPACITY);

  _buttons[FORBID_DIAGONAL_IDX].lockInput();
  _buttons[FORBID_DIAGONAL_IDX].hide();

  _texts[DIAGONAL_TEXT_IDX].create(
      Point(OptionMenuDimensions::MENU_X + 70,
          OptionMenuDimensions::MENU_Y + 50), "Diagonal", Colors::RED,
      FontSize::SMALL);
  _texts[LEVEL_TEXT_IDX].create(
      Point(OptionMenuDimensions::MENU_X + 20,
          OptionMenuDimensions::MENU_Y + 135), "Level", Colors::DARK_OCHRID,
      FontSize::SMALL);
  _texts[LEVEL_NUMBER_TEXT_IDX].create(
      Point(OptionMenuDimensions::MENU_X + 280,
          OptionMenuDimensions::MENU_Y + 135), "1", Colors::DARK_OCHRID,
      FontSize::SMALL);

  _texts[LEVEL_TEXT_IDX].activateAlphaModulation();
  _texts[LEVEL_TEXT_IDX].moveRight(OptionMenuDimensions::MENU_OFFSET_X);
  _texts[LEVEL_TEXT_IDX].setOpacity(OptionMenuDimensions::CLOSED_MENU_OPACITY);

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
    for (auto &text : _texts) {
      text.draw();
    }
  } else {
    _menuImg.draw();
    _buttons[TOGGLE_MENU_IDX].draw();
    _texts[LEVEL_TEXT_IDX].draw();
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
  widgets.reserve(BUTTONS_COUNT + static_cast<int32_t>(TEXTS_COUNT));
  widgets.push_back(&_menuImg);
  for (auto &button : _buttons) {
    if (button.isVisible()) {
      widgets.push_back(& (button.getButtonImage()));
    }
  }
  for (const auto &text : _texts) {
    widgets.push_back(&text);
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

void OptionSelector::setTextLevelId(const int32_t levelId) {
  _texts[LEVEL_NUMBER_TEXT_IDX].setText(std::to_string(levelId + 1).c_str());
  centerLevelValueText();
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
    _texts[DIAGONAL_TEXT_IDX].setColor(Colors::GREEN);
    setOption(Option::DIAGONAL_MOVEMENT, true);
    break;

  case MenuButtonType::FORBID_DIAGONAL:
    _buttons[ALLOW_DIAGONAL_IDX].unlockInput();
    _buttons[ALLOW_DIAGONAL_IDX].show();
    _buttons[FORBID_DIAGONAL_IDX].lockInput();
    _buttons[FORBID_DIAGONAL_IDX].hide();
    _texts[DIAGONAL_TEXT_IDX].setColor(Colors::RED);
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

  _buttons[TOGGLE_MENU_IDX].setOpacity(FULL_OPACITY);
  _menuImg.setOpacity(FULL_OPACITY);
  _texts[LEVEL_TEXT_IDX].setOpacity(FULL_OPACITY);

  //first move the button and menu background image so they can be "baked" into
  //the FBO
  _buttons[TOGGLE_MENU_IDX].moveLeft(OptionMenuDimensions::MENU_OFFSET_X);
  _menuImg.moveLeft(OptionMenuDimensions::MENU_OFFSET_X);
  _texts[LEVEL_TEXT_IDX].moveLeft(OptionMenuDimensions::MENU_OFFSET_X);

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

  _buttons[TOGGLE_MENU_IDX].setOpacity(
      OptionMenuDimensions::CLOSED_MENU_OPACITY);
  _menuImg.setOpacity(OptionMenuDimensions::CLOSED_MENU_OPACITY);
  _texts[LEVEL_TEXT_IDX].setOpacity(OptionMenuDimensions::CLOSED_MENU_OPACITY);

  //the move them
  _buttons[TOGGLE_MENU_IDX].moveRight(OptionMenuDimensions::MENU_OFFSET_X);
  _menuImg.moveRight(OptionMenuDimensions::MENU_OFFSET_X);
  _texts[LEVEL_TEXT_IDX].moveRight(OptionMenuDimensions::MENU_OFFSET_X);
}

void OptionSelector::centerLevelValueText() {
  const int32_t leftBound =
      _buttons[INCR_LEVEL_IDX].getX() + _buttons[INCR_LEVEL_IDX].getWidth();
  const int32_t rightBound = _buttons[DECR_LEVEL_IDX].getX();
  const int32_t targetOffsetX =
      (rightBound - leftBound - _texts[LEVEL_NUMBER_TEXT_IDX].getWidth()) / 2;
  _texts[LEVEL_NUMBER_TEXT_IDX].setX(leftBound + targetOffsetX);
}

