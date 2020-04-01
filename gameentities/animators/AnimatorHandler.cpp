//Corresponding header
#include "AnimatorHandler.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "gameentities/proxies/GameProxyInterface.hpp"
#include "common/CommonDefines.h"
#include "managers/DrawMgr.h"
#include "utils/drawing/Point.h"
#include "utils/EnumClassUtils.hpp"
#include "utils/Log.h"

AnimatorHandler::AnimatorHandler()
    : _gameInterface(nullptr) {

}

int32_t AnimatorHandler::init(GameProxyInterface *gameInterface,
                              GridContainerProxyInterface *gridInterface) {
  int32_t err = EXIT_SUCCESS;
  _gameInterface = gameInterface;

  const Point BIG_BATMAN_START_POS(
      ( (gDrawMgr->getMonitorWidth() - BatmanDimensions::BIG_BATMAN_WIDTH) / 2),
      ( (gDrawMgr->getMonitorHeight() - BatmanDimensions::BIG_BATMAN_HEIGHT) / 2));

  if (EXIT_SUCCESS != _speechAnimator.init(this, BIG_BATMAN_START_POS,
          Textures::BATMAN_BIG, Textures::WIN_DIALOG, Textures::LOSE_DIALOG,
          Timers::SPEECH_WAIT_TIMER_ID)) {
    LOGERR("Error, _generalTextAnimator.init() failed");
    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _pathAnimator.init(this, gridInterface,
            Textures::BATMAN_SMALL, Timers::PATH_TIMER_ID)) {
      LOGERR("Error, _pathAnimator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    if (EXIT_SUCCESS != _scaleAnimator.init(this, BIG_BATMAN_START_POS,
            Textures::BATMAN_BIG, Timers::SCALE_TIMER_ID)) {
      LOGERR("Error, _pathAnimator.init() failed");
      err = EXIT_FAILURE;
    }
  }

  return err;
}

void AnimatorHandler::draw() {
  if (_speechAnimator.isActive()) {
    _speechAnimator.draw();
  }

  if (_pathAnimator.isActive()) {
    _pathAnimator.draw();
  }

  if (_scaleAnimator.isActive()) {
    _scaleAnimator.draw();
  }
}

bool AnimatorHandler::isActive() const {
  return _scaleAnimator.isActive() || _pathAnimator.isActive()
         || _speechAnimator.isActive();
}

void AnimatorHandler::perform(const AnimEvent event, const std::any &args) {
  switch (event) {
  case AnimEvent::SET_SCALE_AMIM_START_TARGET:
    performSetScaleAnimTarget(ScaleAnimTargetType::START_TARGET, args);
    break;

  case AnimEvent::SET_SCALE_AMIM_END_TARGET:
    performSetScaleAnimTarget(ScaleAnimTargetType::END_TARGET, args);
    break;

  case AnimEvent::LOAD_ANIM_PATH:
    performLoadAnimPath(args);
    break;

  case AnimEvent::START_PATH_ANIM:
    _scaleAnimator.activateAnim(ScaleAnimType::DOWNSCALE);
    break;

  case AnimEvent::START_NO_PATH_ANIM:
    _speechAnimator.activateAnim(SpeecAnimType::LOSE);
    break;

  default:
    LOGERR("Error, received unknown AnimEvent type: %d",
        getEnumClassValue(event));
    break;
  }
}

void AnimatorHandler::performSetScaleAnimTarget(const ScaleAnimTargetType type,
                                                const std::any &args) {
  try {
    const auto pos = std::any_cast<Point>(args);
    if (ScaleAnimTargetType::START_TARGET == type) {
      _scaleAnimator.setStartTargetPos(pos);
    } else{
      _scaleAnimator.setEndTargetPos(pos);
    }
  } catch (const std::bad_any_cast &e) {
    LOGERR("any_cast throwed: %s", e.what());
  }
}

void AnimatorHandler::performLoadAnimPath(const std::any &args) {
  try {
    auto path = *std::any_cast<std::vector<Point>*>(args);
    _pathAnimator.loadPath(path);
  } catch (const std::bad_any_cast &e) {
    LOGERR("any_cast throwed: %s", e.what());
  }
}

void AnimatorHandler::onAnimFinished(const AnimType type) {
  switch (type) {
  case AnimType::DOWNSCALE_ANIM:
    _pathAnimator.activateAnim();
    break;

  case AnimType::UPSCALE_ANIM:
    _speechAnimator.activateAnim(SpeecAnimType::WIN);
    break;

  case AnimType::PATH_ANIM:
    _scaleAnimator.activateAnim(ScaleAnimType::UPSCALE);
    break;

  case AnimType::SPEECH_ANIM:
    _gameInterface->onEndAnimFinished();
    break;

  default:
    LOGERR("Error, received unknown AnimEvent type: %d",
        getEnumClassValue(type));
    break;
  }
}

