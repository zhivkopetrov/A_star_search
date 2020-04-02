//Corresponding header
#include "OptionSelector.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "gameentities/proxies/GameProxyInterface.hpp"
#include "sdl/InputEvent.h"
#include "utils/Log.h"

OptionSelector::OptionSelector() : _gameInterface(nullptr) {

}

int32_t OptionSelector::init(GameProxyInterface *gameInterface) {
  _gameInterface = gameInterface;

//  for (size_t i = 0; i < _buttons.size(); ++i) {
//    if (_buttons[i].init()) {
//      LOGERR("Error, _buttons[%zu].init() failed", i);
//      return EXIT_FAILURE;
//    }
//  }

  return EXIT_SUCCESS;
}

void OptionSelector::draw() {
//  for (auto & button : _buttons) {
//    button.draw();
//  }
}

void OptionSelector::handleEvent(const InputEvent &e) {
  if (TouchEvent::KEYBOARD_RELEASE == e.type) {
    switch (e.key) {
        //TODO temporary - remove after buttons are implemented in the OptionSelector
      case Keyboard::KEY_D:
        setOption(Option::DIAGONAL_MOVEMENT, true);
        break;

        //TODO temporary - remove after buttons are implemented in the OptionSelector
      case Keyboard::KEY_UP:
        setOption(Option::LEVEL_CHANGE, 1);
        break;

        //TODO temporary - remove after buttons are implemented in the OptionSelector
      case Keyboard::KEY_DOWN:
        setOption(Option::LEVEL_CHANGE, -1);
        break;
    }
  }
}

void OptionSelector::setOption(const Option option, const std::any &value) {
  _options[option] = value;
  _gameInterface->onOptionChanged(option, value);
}

void OptionSelector::onMenuButtonClicked(const MenuButtonType buttonType) {
  switch (buttonType) {
  default:
    LOGERR("Received unknown MenuButtonType Id: %hhu",
        getEnumClassValue(buttonType));
  }
}


