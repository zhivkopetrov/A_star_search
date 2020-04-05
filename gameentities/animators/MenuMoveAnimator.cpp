//Corresponding header
#include "MenuMoveAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "gameentities/proxies/AnimatorHandlerProxyInterface.hpp"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

MenuMoveAnimator::MenuMoveAnimator()
    : _animatorHandlerInterface(nullptr), _moveTimerId(INIT_INT32_VALUE),
      _animStepsLeft(0), _updateSign(0), _currAnimOpacity(FULL_OPACITY),
      _currAnimStatus(OptionAnimStatus::UNKNOWN), _isActive(false) {

}

int32_t MenuMoveAnimator::init(AnimatorHandlerProxyInterface *interface,
                               const Rectangle &animFBODimensions,
                               const int32_t moveTimerId) {
  _animatorHandlerInterface = interface;
  _moveTimerId = moveTimerId;
  _animContent.create(animFBODimensions);
  _animContent.activateAlphaModulation();
  _animContent.setClearColor(Colors::FULL_TRANSPARENT);
  _currAnimOpacity = OptionMenuDimensions::CLOSED_MENU_OPACITY;
  _animContent.setOpacity(_currAnimOpacity);
  return EXIT_SUCCESS;
}

void MenuMoveAnimator::startAnim(const OptionAnimStatus type) {
  _isActive = true;
  _currAnimStatus = type;
  _animStepsLeft = MAX_ANIM_STEPS;

  if (OptionAnimStatus::START_OPEN_ANIM == _currAnimStatus) {
    _updateSign = 1;
    _animContent.setX(
        OptionMenuDimensions::MENU_X + OptionMenuDimensions::MENU_OFFSET_X);
  } else if (OptionAnimStatus::START_CLOSE_ANIM == _currAnimStatus) {
    _updateSign = -1;
    _animContent.setX(OptionMenuDimensions::MENU_X);
  }
  startTimer(20, _moveTimerId, TimerType::PULSE);
}

void MenuMoveAnimator::draw() {
  _animContent.draw();
}

void MenuMoveAnimator::updateAnimContent(
    const std::vector<const Widget*> &widgets) {
  //move the frame buffer object on target location
  const int32_t currX = _animContent.getX();
  if (currX != OptionMenuDimensions::MENU_X) {
    _animContent.setX(OptionMenuDimensions::MENU_X);
  }

  _animContent.unlock();
  _animContent.reset();

  for (const Widget *widget : widgets) {
    _animContent.addWidget(*widget);
  }

  _animContent.update();
  _animContent.lock();
}

void MenuMoveAnimator::onTimeout(const int32_t timerId) {
  if (_moveTimerId == timerId) {
    processAnim();
  } else {
    LOGERR("Received unknown timerId: %d", timerId);
  }
}

void MenuMoveAnimator::processAnim() {
  --_animStepsLeft;
  if (0 != _animStepsLeft) {
    updateAnimInternals();
  } else {
    if (OptionAnimStatus::START_OPEN_ANIM == _currAnimStatus) {
      _animatorHandlerInterface->onAnimFinished(AnimType::MENU_OPEN_MOVE_ANIM);
    } else if (OptionAnimStatus::START_CLOSE_ANIM == _currAnimStatus) {
      _animatorHandlerInterface->onAnimFinished(AnimType::MENU_CLOSE_MOVE_ANIM);
    }
    _isActive = false;
    _currAnimStatus = OptionAnimStatus::UNKNOWN;
    stopTimer(_moveTimerId);
  }
}

void MenuMoveAnimator::updateAnimInternals() {
  const int32_t offsetX = ANIM_MOVE_STEP * _updateSign;
  _animContent.moveLeft(offsetX);

  _currAnimOpacity += (ANIM_OPACITY_STEP * _updateSign);
  _animContent.setOpacity(_currAnimOpacity);
}

