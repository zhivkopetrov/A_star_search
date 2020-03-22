#ifndef COMMON_COMMONDEFINES_H_
#define COMMON_COMMONDEFINES_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations

namespace Textures {
enum : uint8_t {
  VERTICAL_LINE,
  HORIZONTAL_LINE,
  START_NODE,
  END_NODE,
  WALL,
  A_STAR_PATH,
  FPS_TEXT,
  NO_AVAILABLE_PATH_TEXT,

  COUNT
};
} //namespace Textures

namespace Direction {
enum : uint8_t {
  DOWN, LEFT, RIGHT, UP,

  COUNT
};
} //namespace Direction

namespace FontSize {
enum {
  SMALL, BIG
};
} //namespace FontSize

namespace Movement {
enum {
  //simulate Pythagoras theorem distance by using 10 for the
  //horizontal/vertical movement and
  //14 for diagonal movement, thus evading the cost of using sqrt()
  DIAGONAL_DISTANCE = 14,
  NON_DIAGONAL_DISTANCE = 10,
  DIAGONAL_COMPENSATION = DIAGONAL_DISTANCE - (2 * NON_DIAGONAL_DISTANCE)
};
} //namespace Movement

namespace Timers {
enum {
  PATH_TIMER_ID
};
}

#endif /* COMMON_COMMONDEFINES_H_ */

