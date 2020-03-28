#ifndef GAMEENTITIES_OPTIONSELECTOR_H_
#define GAMEENTITIES_OPTIONSELECTOR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <unordered_map>
#include <any>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"

//Forward declarations
class GameProxyInterface;
class InputEvent;

class OptionSelector {
public:
  OptionSelector();
  virtual ~OptionSelector() = default;

  int32_t init(GameProxyInterface *gameInterface);

  void draw();

  void handleEvent(const InputEvent &e);

  void setOption(const Option option, const std::any &value);

private:
  std::unordered_map<Option, std::any> _options;

  GameProxyInterface *_gameInterface;

  //TODO add buttons and animators
};

#endif /* GAMEENTITIES_OPTIONSELECTOR_H_ */
