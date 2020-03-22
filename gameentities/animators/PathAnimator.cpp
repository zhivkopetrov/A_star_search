//Corresponding header
#include "PathAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "gameentities/GridContainer.h"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

PathAnimator::PathAnimator()
    : _gridContainer(nullptr), _pathTimerId(INIT_INT32_VALUE),
      _isActive(false) {

}

int32_t PathAnimator::init(GridContainer *gridContainer,
                           const int32_t pathTimerId) {
  int32_t err = EXIT_SUCCESS;
  _pathTimerId = pathTimerId;

  if (nullptr != gridContainer) {
    _gridContainer = gridContainer;
  } else {
    LOGERR("Error, nullptr provided for GridContainer inteface");
    err = EXIT_FAILURE;
  }

  return err;
}

void PathAnimator::startAnimation(std::vector<Point> &path) {
  _pathToAnimate.swap(path);
  _isActive = true;

  startTimer(100, _pathTimerId, TimerType::PULSE);
}

void PathAnimator::onTimeout(const int32_t timerId) {
  if (Timers::PATH_TIMER_ID == timerId) {
    if (!_pathToAnimate.empty()) {
      processAnim();
    } else {
      stopTimer(timerId);
      _isActive = false;
    }
  } else {
    LOGERR("Received unknown timerId: %d", timerId);
  }
}

void PathAnimator::processAnim() {
  const Point & currPos = _pathToAnimate.back();
  _gridContainer->addAStarPathNode(currPos.x, currPos.y);
  _pathToAnimate.pop_back();
}
