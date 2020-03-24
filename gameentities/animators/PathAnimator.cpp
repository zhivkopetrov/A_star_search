//Corresponding header
#include "PathAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "gameentities/proxies/AnimatorHandlerProxyInterface.hpp"
#include "common/CommonDefines.h"
#include "gameentities/GridContainer.h"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

PathAnimator::PathAnimator()
    : _animatorHandlerInterface(nullptr), _gridInterface(nullptr), _animStep(0),
      _offsetX(0), _offsetY(0), _pathTimerId(INIT_INT32_VALUE), _isActive(false) {

}

int32_t PathAnimator::init(
    AnimatorHandlerProxyInterface *animatorHandlerInterface,
    GridContainerProxyInterface *gridInterface, const uint8_t batmanRsrcId,
    const int32_t pathTimerId) {
  _animatorHandlerInterface = animatorHandlerInterface;
  _gridInterface = gridInterface;
  _pathTimerId = pathTimerId;
  _batmanImg.create(batmanRsrcId);

  return EXIT_SUCCESS;
}

void PathAnimator::loadPath(std::vector<Point> &path) {
  _pathToAnimate.swap(path);

  const Point initialNodePos = _pathToAnimate.back();
  _batmanImg.setPosition(
      _gridInterface->getNodeCoordinates(initialNodePos.x, initialNodePos.y));
  _batmanImg.moveRight(BatmanDimensions::START_POS_X_OFFSET);

  //remove the last node since it's the start position
  _pathToAnimate.pop_back();
}

void PathAnimator::draw() {
  _batmanImg.draw();
}

void PathAnimator::onScaleAnimFinished() {
  _isActive = true;
  _animStep = ANIM_MOVES;
  loadNextOffsets();
  startTimer(20, _pathTimerId, TimerType::PULSE);
}

void PathAnimator::onTimeout(const int32_t timerId) {
  if (_pathTimerId == timerId) {
    if (!_pathToAnimate.empty()) {
      processAnim();
    } else {
      stopTimer(timerId);
      _isActive = false;
      _animatorHandlerInterface->onAnimFinished();
    }
  } else {
    LOGERR("Received unknown timerId: %d", timerId);
  }
}

void PathAnimator::processAnim() {
  --_animStep;
  moveImage();

  if (0 == _animStep) {
    const Point &currPos = _pathToAnimate.back();
    //we are not interested in end node
    if (1 < _pathToAnimate.size()) {
      _gridInterface->addAStarPathNode(currPos.x, currPos.y);
    }
    _pathToAnimate.pop_back();

    _animStep = ANIM_MOVES;
    loadNextOffsets();
  }
}

void PathAnimator::loadNextOffsets() {
  const Point END_NODE_POS = _pathToAnimate.back();
  const Point END_BATMAN_POS = _gridInterface->getNodeCoordinates(
      END_NODE_POS.x, END_NODE_POS.y);
  _offsetX = (END_BATMAN_POS.x + BatmanDimensions::START_POS_X_OFFSET
      - _batmanImg.getX())
             / ANIM_MOVES;
  _offsetY = (END_BATMAN_POS.y - _batmanImg.getY()) / ANIM_MOVES;
}

void PathAnimator::moveImage() {
  if (_offsetX) {
    _batmanImg.setX(_batmanImg.getX() + _offsetX);
  }

  if (_offsetY) {
    _batmanImg.setY(_batmanImg.getY() + _offsetY);
  }
}

