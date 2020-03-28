//Corresponding header
#include "OptionSelector.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "proxies/GameProxyInterface.hpp"
#include "sdl/InputEvent.h"

OptionSelector::OptionSelector() : _gameInterface(nullptr) {

}

int32_t OptionSelector::init(GameProxyInterface *gameInterface) {
  _gameInterface = gameInterface;

  return EXIT_SUCCESS;
}

void OptionSelector::draw() {

}

void OptionSelector::handleEvent(const InputEvent &) {

}

void OptionSelector::setOption(const Option option, const std::any &value) {
  _options[option] = value;
  _gameInterface->onOptionChanged(option, value);
}


