#ifndef SDL_MOUSEUTILS_H_
#define SDL_MOUSEUTILS_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward Declarations
struct SDL_Surface;

class MouseUtils {
public:
  MouseUtils() = delete;
  ~MouseUtils() = delete;

  /** @brief used to toggle ON/OFF for cursor visual status
   *
   *  @param const int32_t - wanted cursor state
   *             Possible values: 1 to show the cursor
   *                              0 to hide it,
   *                             -1 to query the current state.
   *
   *  @return 0 if the cursor is shown,
   *       or 1 if the cursor is hidden.
   *
   *            NOTE: when invoked with 1 or 0 parameter for cursorStatus
   *                  the return value represent the state state before
   *                  the update.
   *
   *                  Example toggleCursorStatus(0) will return 1
   *                  the first time invoked, if the cursor was visible
   *                  before that.
   * */
  static int32_t toggleCursorStatus(const int32_t cursorStatus);

  /** @brief creates new cursor and hides the system one
   *
   *  @param const char *  - path to new cursor image
   *  @param const int32_t - cursor click X
   *  @param const int32_t - cursor click Y
   *
   *  @return int32_t      - error code
   * */
  static int32_t createCursorFromImg(const char *imagePath,
                                     const int32_t cursorClickX,
                                     const int32_t cursorClickY);

  /** @brief frees memory for the allocated cursor */
  static void freeCursor();
};

#endif /* SDL_MOUSEUTILS_H_ */

