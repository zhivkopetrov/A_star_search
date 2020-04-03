//Corresponding header
#include "MenuButton.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "sdl/InputEvent.h"

MenuButton::MenuButton()
    : _menuInterface(nullptr), _buttonType(MenuButtonType::UNKNOWN) {

}

int32_t MenuButton::init(const MenuButtonCfg &cfg) {
  _menuInterface = cfg.interface;
  _buttonType = cfg.buttonType;
  create(cfg.buttonRsrcId);
  setPosition(cfg.startPos);

  return EXIT_SUCCESS;
}

void MenuButton::handleEvent(const InputEvent &e) {
  if (TouchEvent::TOUCH_PRESS == e.type) {
    setFrame(CLICKED);
  } else if (TouchEvent::TOUCH_RELEASE == e.type) {
    setFrame(UNCLICKED);
    _menuInterface->onMenuButtonClicked(_buttonType);
  }
}

void MenuButton::lockInput() {
  _isInputUnlocked = false;
}

void MenuButton::unlockInput() {
  _isInputUnlocked = true;
}
