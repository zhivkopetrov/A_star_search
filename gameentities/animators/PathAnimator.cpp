//Corresponding header
#include "PathAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "gameentities/GridContainer.h"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

PathAnimator::PathAnimator()
    : _gridInterface(nullptr), _pathTimerId(INIT_INT32_VALUE),
      _isActive(false) {

}

int32_t PathAnimator::init(GridContainerProxyInterface *gridInterface,
                           const uint8_t batmanRsrcId,
                           const int32_t pathTimerId) {
  int32_t err = EXIT_SUCCESS;
  _pathTimerId = pathTimerId;
  _batmanImg.create(batmanRsrcId);

  if (nullptr != gridInterface) {
    _gridInterface = gridInterface;
  } else {
    LOGERR("Error, nullptr provided for GridContainer inteface");
    err = EXIT_FAILURE;
  }

  return err;
}

void PathAnimator::loadPath(std::vector<Point> &path) {
  _pathToAnimate.swap(path);

  //remove the last node since it's the start position
  _pathToAnimate.pop_back();
}

void PathAnimator::draw() {
  _batmanImg.draw();
}

void PathAnimator::onScaleAnimFinished() {
  _isActive = true;
  startTimer(100, _pathTimerId, TimerType::PULSE);
}

void PathAnimator::onTimeout(const int32_t timerId) {
  if (_pathTimerId == timerId) {
    //we are not interested in end node
    if (1 < _pathToAnimate.size()) {
      processAnim();
    } else {
      stopTimer(timerId);
      _pathToAnimate.clear();
      _isActive = false;
    }
  } else {
    LOGERR("Received unknown timerId: %d", timerId);
  }
}

void PathAnimator::processAnim() {
  const Point & currPos = _pathToAnimate.back();
  _gridInterface->addAStarPathNode(currPos.x, currPos.y);
  _pathToAnimate.pop_back();
}
