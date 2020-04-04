#ifndef UTILS_DRAWING_DRAWPARAMS_H_
#define UTILS_DRAWING_DRAWPARAMS_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "Point.h"
#include "Rectangle.h"

enum class WidgetType : uint8_t {
  UNKNOWN,
  IMAGE,
  TEXT,
  FBO
};

enum class BlendMode : uint8_t {
  NONE,  //value for SDL_BLENDMODE_NONE
  BLEND, //value for SDL_BLENDMODE_BLEND
  ADD,   //value for SDL_BLENDMODE_ADD
};

inline constexpr int32_t FULL_OPACITY = 255;
inline constexpr int32_t ZERO_OPACITY = 0;

struct DrawParams {
  DrawParams();

  void reset();

  //Top left position of texture
  Point pos;

  //Holds frame dimensions (x, y, w, h) within the texture resource
  Rectangle frameRect;

  //Draw dimensions of the texture
  int32_t width;
  int32_t height;

  union {
    //unique resource ID of the texture
    int32_t rsrcId;

    //unique resource ID of the texture
    int32_t textId;

    //unique resource ID of the texture
    int32_t FBOId;
  };

  //holds current frame of the texture
  int32_t frameId;

  //holds current opacity of the texture
  int32_t opacity;

  //holds the type of widget that the draw params are related to
  WidgetType widgetType;
};

#endif /* UTILS_DRAWING_DRAWPARAMS_H_ */

