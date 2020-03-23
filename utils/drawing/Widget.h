#ifndef UTILS_DRAWING_WIDGET_H_
#define UTILS_DRAWING_WIDGET_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "DrawParams.h"

//Forward Declarations

/* Common class for graphical Textures.
 * All graphical textures must inherit from Widget */
class Widget {
public:
  Widget();
  ~Widget() = default;

  //move constructor needed for STL containers empalce_back/push_back
  Widget(Widget &&movedOther);

  //move assignment operator implementation
  Widget& operator=(Widget &&movedOther);

  //forbid the copy constructor and copy assignment operator
  Widget(const Widget &other) = delete;
  Widget& operator=(const Widget &other) = delete;

  void draw();

  /** @brief used to set widget position.
   *
   *  @param const int32_t - new X coordinate
   *  @param const int32_t - new Y coordinate
   *         NOTE: the position is relative to the current monitorWindow.
   * */
  inline void setPosition(const int32_t x, const int32_t y) {
    _drawParams.pos.x = x;
    _drawParams.pos.y = y;
  }

  /** @brief used to set widget position.
   *
   *  @param const Point & - new coordinates
   *         NOTE: the position is relative to the current monitorWindow.
   * */
  inline void setPosition(const Point &pos) {
    _drawParams.pos = pos;
  }

  /** @brief used to set widget X coordinate.
   *
   *  @param const int32_t - new X coordinate
   *         NOTE: the position is relative to the current monitorWindow.
   * */
  inline void setX(const int32_t x) {
    _drawParams.pos.x = x;
  }

  /** @brief used to set widget Y coordinate.
   *
   *  @param const int32_t - new Y coordinate
   *         NOTE: the position is relative to the current monitorWindow.
   * */
  inline void setY(const int32_t y) {
    _drawParams.pos.y = y;
  }

  inline void setWidth(const int32_t width) {
    _drawParams.width = width;
  }

  inline void setHeight(const int32_t height) {
    _drawParams.height = height;
  }

  /** @brief used to move widget vertically.
   *
   *  @param const int32_t - Y coordinate offset
   *         NOTE: the position is relative to the current monitorWindow.
   * */
  inline void moveDown(const int32_t y) {
    _drawParams.pos.y += y;
  }

  /** @brief used to move widget vertically.
   *
   *  @param const int32_t - Y coordinate offset
   *         NOTE: the position is relative to the current monitorWindow.
   * */
  inline void moveUp(const int32_t y) {
    _drawParams.pos.y -= y;
  }

  /** @brief used to move widget horizontally.
   *
   *  @param const int32_t - X coordinate offset
   *         NOTE: the position is relative to the current monitorWindow.
   * */
  inline void moveLeft(const int32_t x) {
    _drawParams.pos.x -= x;
  }

  /** @brief used to move widget horizontally.
   *
   *  @param const int32_t - X coordinate offset
   *         NOTE: the position is relative to the current monitorWindow.
   * */
  inline void moveRight(const int32_t x) {
    _drawParams.pos.x += x;
  }

  /** @brief used to get absolute image coordinates
   *
   *  @returns Point - absolute image coordinates
   * */
  inline Point getPosition() const {
    return _drawParams.pos;
  }

  /** @brief used to get absolute image X coordinate
   *
   *  @returns int32_t - absolute image X coordinate
   * */
  inline int32_t getX() const {
    return _drawParams.pos.x;
  }

  /** @brief used to get absolute image Y coordinate
   *
   *  @returns int32_t - absolute image Y coordinate
   * */
  inline int32_t getY() const {
    return _drawParams.pos.y;
  }

  inline int32_t getWidth() const {
    return _drawParams.width;
  }

  inline int32_t getHeight() const {
    return _drawParams.height;
  }

  /** @brief used for making a copy of the current Widget DrawParams
   *
   *  @param DrawParams - current Widget DrawParams
   * */
  inline DrawParams getDrawParams() const {
    return _drawParams;
  }

  /** @brief used to determine whether the widget is created or not
   *
   *  @return bool - is Widget created or not
   * */
  inline bool isCreated() const {
    return _isCreated;
  }

  /** @brief used to hide widget (so it not be unnecessary drawn
   *                                       when draw() method is called)
   * */
  inline void hide() {
    _isVisible = false;
  }

  /** @brief used to show the widget (so it will be drawn
   *                                       when draw() method is called)
   * */
  inline void show() {
    _isVisible = true;
  }

  /** @brief used to determine whether the widget is hidden or not
   *
   *  @return bool - is Widget hidden or not
   * */
  inline bool isVisible() const {
    return _isVisible;
  }

  inline uint8_t getRsrcId() const {
    return _drawParams.rsrcId;
  }

protected:
  void reset();

  //Draw parameters needed for the renderer to perform a draw call
  DrawParams _drawParams;

  //flag to determine if Widget is created in order not to create it twice
  bool _isCreated;

  //flag to determine if Widget is visible or not
  bool _isVisible;
};

#endif /* UTILS_DRAWING_WIDGET_H_ */

