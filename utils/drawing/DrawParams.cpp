//Corresponding header
#include "DrawParams.h"

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "utils/LimitValues.hpp"

DrawParams::DrawParams()
    : pos(Point::UNDEFINED), frameRect(Rectangle::UNDEFINED), width(0),
      height(0), rsrcId(INIT_INT32_VALUE), frameId(0), opacity(FULL_OPACITY),
      widgetType(WidgetType::UNKNOWN) {

}

void DrawParams::reset() {
  pos = Point::UNDEFINED;
  frameRect = Rectangle::UNDEFINED;
  width = 0;
  height = 0;
  rsrcId = INIT_INT32_VALUE;
  frameId = 0;
  opacity = FULL_OPACITY;
  widgetType = WidgetType::UNKNOWN;
}

