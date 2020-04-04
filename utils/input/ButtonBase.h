/*
 * Button.h
 *
 *  Brief: The class should be inherited by every new individual Button class
 *         Example:
 *         #include "ButtonBase.h"
 *         class ExitButton : public ButtonBase
 *         {
 *              virtual void handleEvent(const InputEvent& e) override
 *              {
 *                  //some concrete implementation
 *              }
 *
 *              virtual void onLeave() override
 *              {
 *                  //some concrete implementation
 *              }
 *
 *              virtual void onReturn() override
 *              {
 *                  //some concrete implementation
 *              }
 *         };
 *
 *         NOTE: overriding onLeave() and onReturn() functions are optional.
 *               If it's not overridden the default implementation will be used.
 */

#ifndef MANAGERUTILS_DRAWING_BUTTONBASE_H_
#define MANAGERUTILS_DRAWING_BUTTONBASE_H_

//C system headers

//C++ system headers

//Other libraries headers
#include "TouchEntity.h"

//Own components headers
#include "utils/drawing/Image.h"
#include "utils/drawing/Rectangle.h"

//Forward declarations
class InputEvent;

class ButtonBase: public TouchEntity {
public:
  enum ButtonStates {
    UNCLICKED, CLICKED, DISABLED
  };

  ButtonBase();

  //move constructor needed for STL containers empalce_back/push_back
  ButtonBase(ButtonBase &&movedOther);

  //move assignment operator implementation
  ButtonBase& operator=(ButtonBase &&movedOther);

  //forbid the copy constructor and copy assignment operator
  ButtonBase(const ButtonBase &other) = delete;
  ButtonBase& operator=(const ButtonBase &other) = delete;

  virtual ~ButtonBase() = default;

  /** @brief used to handle user input event down the event hierarchy
   *
   *  @param const Event & - user input event
   * */
  virtual void handleEvent(const InputEvent &e) = 0;

  //=============== START TouchEntity related functions ==================

  /** @brief used to check whether event is inside
   *                                       current frameRectangle or not
   *         NOTE: concrete implementation of TouchEntity function
   *
   *  @param Graphics & - graphics instance
   *
   *  @returns bool     - isInside or not
   * */
  virtual bool containsEvent(const InputEvent &e) override final;

  /** @brief invoked when event was PRESSED inside this TouchEntity
   *         and event leaves the borders of the TouchEntity when no
   *         RELEASE action was performed inside of the same TouchEntity.
   *         NOTE: concrete implementation of TouchEntity function
   *
   *  @param const InputEvent & - engine's InputEvent
   * */
  virtual void onLeave(const InputEvent &e) override;

  /** @brief invoked when event was PRESSED inside this TouchEntity
   *         and event leaves the borders of the TouchEntity when no
   *         RELEASE action was performed inside of the same TouchEntity.
   *         While still action is PRESSED we then again return in the
   *         borders of this TouchEntity.
   *
   *  @param const InputEvent & - engine's InputEvent
   * */
  virtual void onReturn(const InputEvent &e) override;

  /** @brief used to lock this TouchEntity for incoming InputEvent
   * */
  virtual void lockInput() override;

  /** @brief used to unlock this TouchEntity for incoming InputEvent
   * */
  virtual void unlockInput() override;

  //================ END TouchEntity related functions ===================

  //================== START Image related functions =====================

  /** @brief used to create _buttonTexture resource. This function must
   *         be called in order to operate will be resource.
   *         The Image itself only handles draw specific data,
   *         not the actual Surface/Texture.
   *   This function does not return error code for performance reasons
   *
   *  @param const int32_t - unique resource ID
   * */
  void create(const int32_t rsrcId);

  inline void draw() {
    _buttonTexture.draw();
  }

  inline const Image& getButtonImage() const {
    return _buttonTexture;
  }

  inline bool isCreated() {
    return _buttonTexture.isCreated();
  }

  inline void setPosition(const int32_t x, const int32_t y) {
    _originalEventRect.x = x;
    _originalEventRect.y = y;
    _buttonTexture.setPosition(x, y);
  }

  inline void setPosition(const Point &pos) {
    _originalEventRect.x = pos.x;
    _originalEventRect.y = pos.y;
    _buttonTexture.setPosition(pos);
  }

  inline void setX(const int32_t x) {
    _originalEventRect.x = x;
    _buttonTexture.setX(x);
  }

  inline void setY(const int32_t y) {
    _originalEventRect.y = y;
    _buttonTexture.setY(y);
  }

  inline void setFrame(const int32_t frameIndex) {
    _buttonTexture.setFrame(frameIndex);
  }

  inline void setOpacity(const int32_t opacity) {
    _buttonTexture.setOpacity(opacity);
  }

  inline void moveDown(const int32_t y) {
    _originalEventRect.y += y;
    _buttonTexture.moveDown(y);
  }

  inline void moveUp(const int32_t y) {
    _originalEventRect.y -= y;
    _buttonTexture.moveUp(y);
  }

  inline void moveLeft(const int32_t x) {
    _originalEventRect.x -= x;
    _buttonTexture.moveLeft(x);
  }

  inline void moveRight(const int32_t x) {
    _originalEventRect.x += x;
    _buttonTexture.moveRight(x);
  }

  inline Point getPosition() const {
    return Point(_originalEventRect.x, _originalEventRect.y);
  }

  inline int32_t getX() const {
    return _originalEventRect.x;
  }

  inline int32_t getY() const {
    return _originalEventRect.y;
  }

  inline int32_t getWidth() const {
    return _originalEventRect.w;
  }

  inline int32_t getHeight() const {
    return _originalEventRect.h;
  }

  int32_t getOpacity() const {
    return _buttonTexture.getOpacity();
  }

  Rectangle getButtonEventRect() const;

  inline void hide() {
    _buttonTexture.hide();
  }

  inline void show() {
    _buttonTexture.show();
  }

  inline bool isVisible() const {
    return _buttonTexture.isVisible();
  }

  //=================== END Image related functions ======================

  /** @brief used to set user defined capture event rectangle. If such
   *         is not provided the default image frame rectangle is used
   *
   *  @param const Rectangle & - user provided capture event rectangle
   *
   *  WARNING: _captureEventRect is absolute. It is not affected by moving
   *  functions (e.g. setPosition/setX/setY/moveLeft/moveRight etc... )
   * */
  void setEventCaptureRect(const Rectangle &rect);

  /** @brief used to reset user defined capture event rectangle. By doing
   *         so the default image frame rectangle will be used
   * */
  void resetEventCaptureRect();

  /** @brief used to check whether user defined capture event rectangle
   *                                                               is set.
   *
   *  @return bool - user provided capture event rectangle is set or not
   * */
  inline bool isEventCaptureRectSet() const {
    return _isCaptureEventRectSet;
  }

  inline void activateAlphaModulation() {
    _buttonTexture.activateAlphaModulation();
  }

  inline void deactivateAlphaModulation() {
    _buttonTexture.deactivateAlphaModulation();
  }

private:
  /** Holds the graphical button representation texture
   * */
  Image _buttonTexture;

  /** Holds original capture event rectangle(_buttonTexture frame
   *                                                         dimensions).
   * */
  Rectangle _originalEventRect;

  /** Holds user defines capture event rectangle. If such is not provided
   *  the default _buttonTexture frame rectangle is used.
   *
   *  WARNING: _captureEventRect is absolute. It is not affected by moving
   *  functions (e.g. setPosition/setX/setY/moveLeft/moveRight etc... )
   * */
  Rectangle _captureEventRect;

  /** Used to check whether _captureEventRect is set or not.
   *
   *  NOTE: this bool variable can be skipped, but it's better to check
   *  if(_isCaptureEventRectSet) rather than
   *  if(Rectangle::UNDEFINED == _captureEventRect), which internally is
   *  8 integers comparison
   * */
  bool _isCaptureEventRectSet;
};

#endif /* MANAGERUTILS_DRAWING_BUTTONBASE_H_ */

