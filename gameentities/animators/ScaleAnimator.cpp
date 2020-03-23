//Corresponding header
#include "ScaleAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cmath>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "gameentities/proxies/PathAnimatorProxyInterface.hpp"
#include "managers/DrawMgr.h"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

ScaleAnimator::ScaleAnimator()
    : _pathAnimInterface(nullptr), _imgOrigWidth(0), _imgOrigHeight(0),
      _remainingSteps(0), _xOffset(0), _yOffset(0), _remainderX(0),
      _remainderY(0), _scaleXOffset(0), _scaleYOffset(0), _remainderScaleX(0),
      _remainderScaleY(0), _scaleTimerId(INIT_INT32_VALUE), _isActive(false) {

}

int32_t ScaleAnimator::init(PathAnimatorProxyInterface *pathAnimInterface,
                            const uint8_t batmanRsrcId,
                            const int32_t scaleTimerId) {
  _pathAnimInterface = pathAnimInterface;
  _batman.create(batmanRsrcId);
  _imgOrigWidth = _batman.getWidth();
  _imgOrigHeight = _batman.getHeight();
  _scaleTimerId = scaleTimerId;
  return EXIT_SUCCESS;
}

void ScaleAnimator::draw() {
  _batman.draw();
}

void ScaleAnimator::setTargetPos(const Point &pos) {
  _endPos = pos;
  //adjust start X, because the image is smaller than the tile
  _endPos.x += START_POS_X_OFFSET;
  calculateAnimInternals();
}

void ScaleAnimator::startAnim() {
  _isActive = true;

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
    _batman.setWidth(_imgOrigWidth);
    _batman.setHeight(_imgOrigHeight);
    _pathAnimInterface->onScaleAnimFinished();
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
  const int32_t MONITOR_WIDTH = gDrawMgr->getMonitorWidth();
  const int32_t MONITOR_HEIGHT = gDrawMgr->getMonitorHeight();
  const int32_t START_X = (MONITOR_WIDTH - BatmanDimensions::BIG_BATMAN_WIDTH)
      / 2;
  const int32_t START_Y = (MONITOR_HEIGHT - BatmanDimensions::BIG_BATMAN_HEIGHT)
      / 2;
  _batman.setPosition(START_X, START_Y);

  const int32_t DELTA_X = _endPos.x - START_X;
  const int32_t DELTA_Y = _endPos.y - START_Y;
  _remainingSteps = std::max(std::abs(DELTA_X), std::abs(DELTA_Y))
      / ANIM_STEP_DISTANCE;

  _xOffset = DELTA_X / _remainingSteps;
  _yOffset = DELTA_Y / _remainingSteps;
  _remainderX = DELTA_X - (_xOffset * _remainingSteps);
  _remainderY = DELTA_Y - (_yOffset * _remainingSteps);

  constexpr int32_t SCALE_DELTA_X = BatmanDimensions::BIG_BATMAN_WIDTH
      - BatmanDimensions::SMALL_BATMAN_WIDTH;
  constexpr int32_t SCALE_DELTA_Y = BatmanDimensions::BIG_BATMAN_HEIGHT
      - BatmanDimensions::SMALL_BATMAN_HEIGHT;

  _scaleXOffset = SCALE_DELTA_X / _remainingSteps;
  _scaleYOffset = SCALE_DELTA_Y / _remainingSteps;
  _remainderScaleX = SCALE_DELTA_X - (_scaleXOffset * _remainingSteps);
  _remainderScaleY = SCALE_DELTA_Y - (_scaleYOffset * _remainingSteps);
}

