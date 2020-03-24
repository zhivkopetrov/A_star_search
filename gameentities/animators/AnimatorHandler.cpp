//Corresponding header
#include "AnimatorHandler.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "utils/drawing/Point.h"
#include "utils/Log.h"

int32_t AnimatorHandler::init(GridContainerProxyInterface *gridInterface) {
  int32_t err = EXIT_SUCCESS;

  if (EXIT_SUCCESS != _generalTextAnimator.init()) {
    LOGERR("Error, _generalTextAnimator.init() failed");
    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _pathAnimator.init(gridInterface,
            Textures::BATMAN_SMALL, Timers::PATH_TIMER_ID)) {
      LOGERR("Error, _pathAnimator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _scaleAnimator.init(&_pathAnimator,
            Textures::BATMAN_BIG, Timers::SCALE_TIMER_ID)) {
      LOGERR("Error, _pathAnimator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  return err;
}

void AnimatorHandler::draw() {
  _generalTextAnimator.draw();

  if (_pathAnimator.isActive()) {
    _pathAnimator.draw();
  }

  if (_scaleAnimator.isActive()) {
    _scaleAnimator.draw();
  }
}

bool AnimatorHandler::isActive() const {
  return _scaleAnimator.isActive() || _pathAnimator.isActive();
}

void AnimatorHandler::perform(const AnimEvent event, const std::any & args) {
  switch (event) {
  case AnimEvent::SET_SCALE_AMIM_TARGET:
    performSetScaleAnimTarget(args);
    break;

  case AnimEvent::LOAD_ANIM_PATH:
    performLoadAnimPath(args);
    break;

  case AnimEvent::START_PATH_ANIM:
    _generalTextAnimator.hideNoPathText();
    _scaleAnimator.startAnim();
    break;

  case AnimEvent::START_NO_PATH_ANIM:
    _generalTextAnimator.showNoPathText();
    break;

  default:
    LOGERR("Error, received unknown AnimEvent type: %d",
        static_cast<int32_t>(event));
    break;
  }
}

void AnimatorHandler::performSetScaleAnimTarget(const std::any & args) {
  try {
    const auto pos = std::any_cast<Point>(args);
    _scaleAnimator.setTargetPos(pos);
  }
  catch (const std::bad_any_cast& e) {
    LOGERR("any_cast throwed: %s\n", e.what());
  }
}

void AnimatorHandler::performLoadAnimPath(const std::any & args) {
  try {
    auto path = *std::any_cast<std::vector<Point> *>(args);
    _pathAnimator.loadPath(path);
  }
  catch (const std::bad_any_cast& e) {
    LOGERR("any_cast throwed: %s\n", e.what());
  }
}


