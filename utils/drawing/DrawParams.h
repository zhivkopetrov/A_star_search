#ifndef UTILS_DRAWING_DRAWPARAMS_H_
#define UTILS_DRAWING_DRAWPARAMS_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "Point.h"

struct DrawParams {
  DrawParams();

  void reset();

  //Top left position of texture
  Point pos;

  //Draw dimensions of the texture
  int32_t width;
  int32_t height;

  //unique resource ID of the texture
  uint8_t rsrcId;

  //holds current frame of the texture
  uint8_t frameId;
};

#endif /* UTILS_DRAWING_DRAWPARAMS_H_ */

