#ifndef UTILS_DRAWING_TEXT_H_
#define UTILS_DRAWING_TEXT_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "Widget.h"
#include "utils/drawing/Color.h"

//Forward declarations

class Text : public Widget {
public:
  Text();
  ~Text();

  Text(Text &&movedOther) = default;
  Text& operator=(Text &&movedOther) = default;

  //forbid the copy constructor and copy assignment operator
  Text(const Text &other) = delete;
  Text& operator=(const Text &other) = delete;

  void create(const Point &startPoint,
              const char *text,
              const Color& color,
              const int32_t fontSize);

  void setText(const char *text);

  void destroy();

private:
  Color _color;
  int32_t _fontSize;

  /* used in order to check if resource was destroyed ->
   *                                              not to destroy it twice
   */
  bool    _isDestroyed;
};

#endif /* UTILS_DRAWING_TEXT_H_ */

