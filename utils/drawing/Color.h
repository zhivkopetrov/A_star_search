#ifndef UTILS_DRAWING_COLOR_H_
#define UTILS_DRAWING_COLOR_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

struct RGBA {
  RGBA() {
    reset();
  }

  void reset() {
    r = 0;
    g = 0;
    b = 0;
    a = 255;
  }

  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
};

class Color {
public:
  /** Warning: if you use this constructor be sure to provide proper
   *           32bit RGBA color.
   *
   *           24bit RGB colors will be misinterpreted. If you have
   *           24bit RGB color hex that you want to use simply add
   *           two trailing FF's at the end of your 24bit hex value.
   *
   *  Example: 24bit RGB hex: 0xF100A3   should be transformed to
   *                          0xF100A3FF
   *           before passed to this constructor
   * */
  explicit Color(const uint32_t inputRgba);

  explicit Color(const uint8_t red, const uint8_t green, const uint8_t blue,
                 const uint8_t alpha = 255);

  //forbid default constructor
  Color() = delete;

  inline bool operator==(const Color &other) const {
    return rgba.r == other.rgba.r && rgba.g == other.rgba.g
           && rgba.b == other.rgba.b && rgba.a == other.rgba.a;
  }

  inline bool operator!=(const Color &other) const {
    return ! (*this == other);
  }

  inline uint32_t get32BitRGBA() const {
    return (rgba.r & 0xFF000000) | (rgba.g & 0x00FF0000) | (rgba.b & 0x0000FF00)
           | (rgba.a & 0x000000FF);
  }

  RGBA rgba;
};

namespace Colors {
extern const Color RED;
extern const Color GREEN;
extern const Color BLUE;
extern const Color BLACK;
extern const Color WHITE;
extern const Color GRAY;
extern const Color CYAN;
extern const Color MAGENTA;
extern const Color YELLOW;
extern const Color ORANGE;
extern const Color PURPLE;
extern const Color DARK_OCHRID;
extern const Color DARK_GREEN;
extern const Color CARAMEL_GOLD;
extern const Color FULL_TRANSPARENT;
} //namespace Colors

#endif /* UTILS_DRAWING_COLOR_H_ */

