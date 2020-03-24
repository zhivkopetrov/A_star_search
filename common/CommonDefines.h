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
  BATMAN_BIG,
  BATMAN_SMALL,
  WIN_DIALOG,
  LOSE_DIALOG,
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

namespace Grid {
enum {
  GRID_HEIGHT = 9,
  GRID_WIDTH = 16,

  TILE_DIMENSION = 115,
  LINE_OFFSET = 5,
  TILE_OFFSET = LINE_OFFSET + TILE_DIMENSION
};
} //namespace Grid

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
  PATH_TIMER_ID,
  SCALE_TIMER_ID,
  SPEECH_WAIT_TIMER_ID
};
} //namespace Timers

namespace BatmanDimensions {
enum {
  SMALL_BATMAN_WIDTH = 66,
  SMALL_BATMAN_HEIGHT = 115,
  BIG_BATMAN_WIDTH = 340,
  BIG_BATMAN_HEIGHT = 590,

  //adjust start X, because the image is smaller than the tile
  START_POS_X_OFFSET = (Grid::TILE_DIMENSION - SMALL_BATMAN_WIDTH) / 2
};
} //namespace BatmanDimensions

namespace SpeechDimensions {
enum {
  WIN_DIALOG_WIDTH = 600,
  WIN_DIALOG_HEIGHT = 440,
  LOSE_DIALOG_WIDTH = 600,
  LOSE_DIALOG_HEIGHT = 400
};
} //namespace BatmanDimensions

#endif /* COMMON_COMMONDEFINES_H_ */

