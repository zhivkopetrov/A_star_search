//Corresponding header
#include "ScaleAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cmath>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "gameentities/proxies/AnimatorHandlerProxyInterface.hpp"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

ScaleAnimator::ScaleAnimator()
    : _animatorHandlerInterface(nullptr), _remainingSteps(0), _xOffset(0),
      _yOffset(0), _remainderX(0), _remainderY(0), _scaleXOffset(0),
      _scaleYOffset(0), _remainderScaleX(0), _remainderScaleY(0),
      _scaleTimerId(INIT_INT32_VALUE), _currAnimType(ScaleAnimType::UNKNOWN),
      _isActive(false) {

}

int32_t ScaleAnimator::init(
    AnimatorHandlerProxyInterface *animatorHandlerInterface,
    const Point &startBatmanPos, const int32_t batmanRsrcId,
    const int32_t scaleTimerId) {
  _animatorHandlerInterface = animatorHandlerInterface;
  _batman.create(batmanRsrcId);
  _scaleTimerId = scaleTimerId;
  _origBatmanPos = startBatmanPos;

  return EXIT_SUCCESS;
}

void ScaleAnimator::draw() {
  _batman.draw();
}

void ScaleAnimator::setStartTargetPos(const Point &pos) {
  _startPos = pos;
  //adjust start X, because the image is smaller than the tile
  _startPos.x += BatmanDimensions::START_POS_X_OFFSET;
}

void ScaleAnimator::setEndTargetPos(const Point &pos) {
  _endPos = pos;
  //adjust start X, because the image is smaller than the tile
  _endPos.x += BatmanDimensions::START_POS_X_OFFSET;
}

void ScaleAnimator::activateAnim(const ScaleAnimType type) {
  _isActive = true;
  _currAnimType = type;
  calculateAnimInternals();

  startTimer(20, _scaleTimerId, TimerType::PULSE);
}

void ScaleAnimator::onTimeout(const int32_t timerId) {
  if (_scaleTimerId == timerId) {
    processAnim();
  } else {
    LOGERR("Received unknown timerId: %d", timerId);
  }
}

void ScaleAnimator::processAnim() {
  --_remainingSteps;
  moveImage();
  scaleImage();

  if (0 == _remainingSteps) {
    _isActive = false;
    stopTimer(_scaleTimerId);

    const AnimType animType =
        (ScaleAnimType::DOWNSCALE == _currAnimType) ?
            AnimType::DOWNSCALE_ANIM : AnimType::UPSCALE_ANIM;
    _animatorHandlerInterface->onAnimFinished(animType);
    _currAnimType = ScaleAnimType::UNKNOWN;
  }
}

void ScaleAnimator::moveImage() {
  int xOffset = _xOffset;
  int yOffset = _yOffset;

  if (_remainderX) {
    if (0 < _remainderX) {
      ++xOffset;
      --_remainderX;
    } else {
      --xOffset;
      ++_remainderX;
    }
  }

  if (_remainderY) {
    if (0 < _remainderY) {
      ++yOffset;
      --_remainderY;
    } else {
      --yOffset;
      ++_remainderY;
    }
  }

  _batman.setX(_batman.getX() + xOffset);
  _batman.setY(_batman.getY() + yOffset);
}

void ScaleAnimator::scaleImage() {
  int scaleOffsetX = _scaleXOffset;
  int scaleOffsetY = _scaleYOffset;

  if (_remainderScaleX) {
    if (0 < _remainderScaleX) {
      ++scaleOffsetX;
      --_remainderScaleX;
    } else {
      --scaleOffsetX;
      ++_remainderScaleX;
    }
  }

  if (_remainderScaleY) {
    if (0 < _remainderScaleY) {
      ++scaleOffsetY;
      --_remainderScaleY;
    } else {
      --scaleOffsetY;
      ++_remainderScaleY;
    }
  }

  _batman.setWidth(_batman.getWidth() - scaleOffsetX);
  _batman.setHeight(_batman.getHeight() - scaleOffsetY);
}

void ScaleAnimator::calculateAnimInternals() {

  Point sourceMove;
  Point targetMove;
  int32_t sourceScaleWidth = 0;
  int32_t targetScaleWidth = 0;
  int32_t sourceScaleHeight = 0;
  int32_t targetScaleHeight = 0;
  if (ScaleAnimType::DOWNSCALE == _currAnimType) {
    _batman.setPosition(_origBatmanPos);
    sourceMove = _origBatmanPos;
    targetMove = _startPos;
    sourceScaleWidth = BatmanDimensions::BIG_BATMAN_WIDTH;
    targetScaleWidth = BatmanDimensions::SMALL_BATMAN_WIDTH;
    sourceScaleHeight = BatmanDimensions::BIG_BATMAN_HEIGHT;
    targetScaleHeight = BatmanDimensions::SMALL_BATMAN_HEIGHT;
  } else if (ScaleAnimType::UPSCALE == _currAnimType) {
    _batman.setPosition(_endPos);
    sourceMove = _endPos;
    targetMove = _origBatmanPos;
    sourceScaleWidth = BatmanDimensions::SMALL_BATMAN_WIDTH;
    targetScaleWidth = BatmanDimensions::BIG_BATMAN_WIDTH;
    sourceScaleHeight = BatmanDimensions::SMALL_BATMAN_HEIGHT;
    targetScaleHeight = BatmanDimensions::BIG_BATMAN_HEIGHT;
  }

  const int32_t moveDeltaX = targetMove.x - sourceMove.x;
  const int32_t moveDeltaY = targetMove.y - sourceMove.y;
  const int32_t scaleDeltaX = sourceScaleWidth - targetScaleWidth;
  const int32_t scaleDeltaY = sourceScaleHeight - targetScaleHeight;

  _remainingSteps = TOTAL_ANIM_STEPS;

  _xOffset = moveDeltaX / _remainingSteps;
  _yOffset = moveDeltaY / _remainingSteps;
  _remainderX = moveDeltaX - (_xOffset * _remainingSteps);
  _remainderY = moveDeltaY - (_yOffset * _remainingSteps);

  _scaleXOffset = scaleDeltaX / _remainingSteps;
  _scaleYOffset = scaleDeltaY / _remainingSteps;
  _remainderScaleX = scaleDeltaX - (_scaleXOffset * _remainingSteps);
  _remainderScaleY = scaleDeltaY - (_scaleYOffset * _remainingSteps);
}

