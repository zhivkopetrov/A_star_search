#ifndef UTILS_DRAWING_TEXT_H_
#define UTILS_DRAWING_TEXT_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "Widget.h"

//Forward declarations

class Text : public Widget {
public:
  Text();
  virtual ~Text() = default;

  //move constructor needed for STL containers emplace_back/push_back
  Text(Text &&movedOther) = default;

  //move assignment operator implementation
  Text& operator=(Text &&movedOther) = default;

  //forbid the copy constructor and copy assignment operator
  Text(const Text &other) = delete;
  Text& operator=(const Text &other) = delete;

  void create(const uint8_t rsrcId,
              const Point &startPoint,
              const char *startText,
              const int32_t fontSize);

  void setText(const char *text);

private:
  int32_t _fontSize;
};

#endif /* UTILS_DRAWING_TEXT_H_ */

