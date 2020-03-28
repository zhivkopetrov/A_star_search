//Corresponding header
#include "ButtonBase.h"

//C system headers

//C++ system headers
#include <utility>

//Other libraries headers
#include "sdl/InputEvent.h"

//Own components headers
#include "utils/Log.h"

//default constructor
ButtonBase::ButtonBase()
    : _originalEventRect(Rectangle::UNDEFINED),
      _captureEventRect(Rectangle::UNDEFINED), _isCaptureEventRectSet(false) {

}

//move constructor
ButtonBase::ButtonBase(ButtonBase &&movedOther) {
  //take ownership of resources
  _buttonTexture = std::move(movedOther._buttonTexture);
  _originalEventRect = movedOther._originalEventRect;
  _captureEventRect = movedOther._captureEventRect;
  _isCaptureEventRectSet = movedOther._isCaptureEventRectSet;
  _isInputUnlocked = movedOther._isInputUnlocked;

  if (!_isCaptureEventRectSet) {
    touchEntityEventRect = &_originalEventRect;
  } else {
    touchEntityEventRect = &_captureEventRect;
  }

  //ownership of resource should be taken from moved instance
  movedOther._originalEventRect = Rectangle::UNDEFINED;
  movedOther._captureEventRect = Rectangle::UNDEFINED;
  movedOther._isCaptureEventRectSet = false;
  movedOther.touchEntityEventRect = nullptr;
  movedOther._isInputUnlocked = true;
}

//move assignment operator
ButtonBase& ButtonBase::operator=(ButtonBase &&movedOther) {
  //check for self-assignment
  if (this != &movedOther) {
    //take ownership of resources
    _buttonTexture = std::move(movedOther._buttonTexture);
    _originalEventRect = movedOther._originalEventRect;
    _captureEventRect = movedOther._captureEventRect;
    _isCaptureEventRectSet = movedOther._isCaptureEventRectSet;
    _isInputUnlocked = movedOther._isInputUnlocked;

    if (!_isCaptureEventRectSet) {
      touchEntityEventRect = &_originalEventRect;
    } else {
      touchEntityEventRect = &_captureEventRect;
    }

    //ownership of resource should be taken from moved instance
    movedOther._originalEventRect = Rectangle::UNDEFINED;
    movedOther._captureEventRect = Rectangle::UNDEFINED;
    movedOther._isCaptureEventRectSet = false;
    movedOther.touchEntityEventRect = nullptr;
    movedOther._isInputUnlocked = true;
  }

  return *this;
}

bool ButtonBase::containsEvent(const InputEvent &e) {
  //you can not click what your don't see. Logical isn't it?
  if (!_buttonTexture.isVisible()) {
    return false;
  }

  if (!isInputUnlocked()) //sanity check
  {
    LOGERR(
        "Warning, you should not call this function when your "
        "TouchEntity is locked. Consider using "
        "TouchEntity::isInputUnlocked() first.");
    return false;
  }

  const Rectangle &usedRect =
      _isCaptureEventRectSet ? _captureEventRect : _originalEventRect;

  const bool isInside = Rectangle::isPointInRect(e.pos, usedRect);
  if (isInside && TouchEvent::TOUCH_PRESS == e.type) {
    e.setLastClicked(this);
  }
  return isInside;
}

void ButtonBase::onLeave(const InputEvent&) {
  _buttonTexture.setFrame(UNCLICKED);
}

void ButtonBase::onReturn(const InputEvent&) {
  _buttonTexture.setFrame(CLICKED);
}

void ButtonBase::lockInput() {
  _isInputUnlocked = false;
  _buttonTexture.setFrame(DISABLED);
}

void ButtonBase::unlockInput() {
  _isInputUnlocked = true;
  _buttonTexture.setFrame(UNCLICKED);
}

void ButtonBase::create(const uint8_t rsrcId) {
  _buttonTexture.create(rsrcId);

  _originalEventRect.x = _buttonTexture.getX();
  _originalEventRect.y = _buttonTexture.getY();
  _originalEventRect.w = _buttonTexture.getWidth();
  _originalEventRect.h = _buttonTexture.getHeight();

  touchEntityEventRect = &_originalEventRect;
}

void ButtonBase::setEventCaptureRect(const Rectangle &rect) {
  _isCaptureEventRectSet = true;
  _captureEventRect = rect;

  /** User defined capture event rectangle is set ->
   * set TouchEntityEventRect to point to it.
   * */
  touchEntityEventRect = &_captureEventRect;
}

/** @brief used to reset user defined capture event rectangle. By doing
 *         so the default image frame rectangle will be used
 * */
void ButtonBase::resetEventCaptureRect() {
  _isCaptureEventRectSet = false;
  _captureEventRect = Rectangle::UNDEFINED;

  /** User defined capture event rectangle is reset ->
   * set TouchEntityEventRect to point to _originalEventRect.
   * */
  touchEntityEventRect = &_originalEventRect;
}

Rectangle ButtonBase::getButtonEventRect() const {
  if (_isCaptureEventRectSet) {
    return _captureEventRect;
  }

  return _originalEventRect;
}

