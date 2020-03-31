#ifndef UTILS_DRAWING_FBO_H_
#define UTILS_DRAWING_FBO_H_

//Forward declarations

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "Widget.h"

//Forward declarations

class FBO: public Widget {
public:
  FBO();
  ~FBO();

  FBO(FBO &&movedOther) = default;
  FBO& operator=(FBO &&movedOther) = default;

  //forbid the copy constructor and copy assignment operator
  FBO(const FBO &other) = delete;
  FBO& operator=(const FBO &other) = delete;

  /** @brief used to create an empty FBO with the given params.
   *
   *  @param const Point & - starting coordinate for the FBO
   *  @param const int32_t - width for the FBO (final texture)
   *  @param const int32_t - height for the FBO (final texture)
   * */
  void create(const Point &startPoint, const int32_t FBOWidth,
              const int32_t FBOHeight);

  /** @brief used to destroy the FBO
   * */
  void destroy();

  /** @brief used for unlocking the FBO before invoking ::update() method
   * */
  void unlock();

  /** @brief used for locking the FBO after ::update() method has been executed
   * */
  void lock();

  /** @brief used to clear (wipe out) the content of the current
   *         FBO and clear the content of _storedItems std::vector
   *
   *         NOTE: each call to ::reset() method must be wrapped between
   *         ::unlock() and ::lock() methods
   * */
  void reset();

  /** @brief used to upload new Graphical Widget in the FBO.
   *         The Widget can represent Image/Text/FBO
   *
   *  @param const Widget & - the Widget that is about to be uploaded
   *                                                  to the FBO
   *
   *         WARNING:  Be careful if you invoke addWidget() for another
   *                   FBO as a parameter, because FBO's,
   *                   deallocated memory for their surfaces/textures
   *                   when destroyed.
   *                   If you happen to call addWidget() for another
   *                   FBO be sure to not call the ::update()
   *                   method if the destroyed FBO DrawParams
   *                   are still part of the _storedItem std::vector.
   *                   It is safe however to call ::reset() and then
   *                   continue with your business logic.
   *
   *         WARNING2: Widget params needs to be created before addWidget()
   *                   method is invoked. If it was not created - the
   *                   upload to the FBO will fail (will be skipped)
   * */
  void addWidget(const Widget &widget);

  /** @brief used to override the existing FBO final texture
   *         with Surfaces/Textures from the _storedItems std::vector
   *
   *         NOTE: each call to ::update() method must be wrapped between
   *         ::unlock() and ::lock() methods
   *  */
  void update();

  void draw();

  /** @brief used to move all stored FBO items (widgets)
   *         with relative offset
   *
   *  @param const int32_t - relative X offset
   * */
  inline void moveItemsRight(const int32_t x) {
    _itemsOffsetX += x;
  }

  /** @brief used to move all stored FBO items (widgets)
   *         with relative offset
   *
   *  @param const int32_t - relative X offset
   * */
  inline void moveItemsLeft(const int32_t x) {
    _itemsOffsetX -= x;
  }

  /** @brief used to move all stored FBO items (widgets)
   *         with relative offset
   *
   *  @param const int32_t - relative Y offset
   * */
  inline void moveItemsDown(const int32_t y) {
    _itemsOffsetY += y;
  }

  /** @brief used to move all stored FBO items (widgets)
   *         with relative offset
   *
   *  @param const int32_t - relative Y offset
   * */
  inline void moveItemsUp(const int32_t y) {
    _itemsOffsetY -= y;
  }

  inline size_t getStoredItemsCount() const {
    return _storedItems.size();
  }

private:
  /** @brief used to transform relative sFBO coordinates to
   *         relative ones for the monitor, on which the FBO is attached to
   * */
  void transformToMonitorRelativeCoordinates();

  /** @brief used to reset internal variables
   * */
  void resetInternals();

  /** Holds all Widget DrawParams used by ::addWidget() method.
   * When ::update() is called the final Surface/Texture is created from
   * all stored items
   * */
  std::vector<DrawParams> _storedItems;

  /* Stores relative offset for stored items(widgets) in the FBO
   * */
  int32_t _itemsOffsetX;
  int32_t _itemsOffsetY;

  /* Holds locked/unlocked status of the FBO */
  bool _isLocked;

  /* Used to determine whether the FBO has already been destroyed
   * */
  bool _isDestroyed;
};

#endif /* UTILS_DRAWING_FBO_H_ */

