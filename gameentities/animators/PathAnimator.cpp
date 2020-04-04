//Corresponding header
#include "PathAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "gameentities/proxies/AnimatorHandlerProxyInterface.hpp"
#include "gameentities/proxies/GridContainerProxyInterface.hpp"
#include "common/CommonDefines.h"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

PathAnimator::PathAnimator()
    : _animatorHandlerInterface(nullptr), _gridInterface(nullptr), _animStep(0),
      _offsetX(0), _offsetY(0), _pathTimerId(INIT_INT32_VALUE), _isActive(false) {

}

int32_t PathAnimator::init(
    AnimatorHandlerProxyInterface *animatorHandlerInterface,
    GridContainerProxyInterface *gridInterface, const int32_t batmanRsrcId,
    const int32_t pathTimerId) {
  _animatorHandlerInterface = animatorHandlerInterface;
  _gridInterface = gridInterface;
  _pathTimerId = pathTimerId;
  _batmanImg.create(batmanRsrcId);

  return EXIT_SUCCESS;
}

void PathAnimator::loadPath(std::vector<Point> &path) {
  _pathToAnimate.swap(path);
  _batmanImg.setPosition(
      _gridInterface->getNodeCoordinates(_pathToAnimate.back()));
  _batmanImg.moveRight(BatmanDimensions::START_POS_X_OFFSET);

  //remove the last node since it's the start position
  _pathToAnimate.pop_back();
}

void PathAnimator::draw() {
  _batmanImg.draw();
}

void PathAnimator::activateAnim() {
  _isActive = true;
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
      _animatorHandlerInterface->onAnimFinished(AnimType::PATH_ANIM);
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
      _gridInterface->addPathNode(currPos);
    }
    _pathToAnimate.pop_back();

    if (!_pathToAnimate.empty()) {
      loadNextOffsets();
    }
  }
}

void PathAnimator::loadNextOffsets() {
  _animStep = ANIM_MOVES;
  const Point END_BATMAN_POS = _gridInterface->getNodeCoordinates(
      _pathToAnimate.back());
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

