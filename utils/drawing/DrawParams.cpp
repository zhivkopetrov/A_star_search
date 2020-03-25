//Corresponding header
#include "DrawParams.h"

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "utils/LimitValues.hpp"

DrawParams::DrawParams()
    : pos(Point::UNDEFINED), width(0), height(0), rsrcId(INIT_UINT8_VALUE),
      frameId(0) {

}

void DrawParams::reset() {
  pos = Point::UNDEFINED;
  width = 0;
  height = 0;
  rsrcId = INIT_UINT8_VALUE;
  frameId = 0;
}

