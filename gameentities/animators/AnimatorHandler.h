#ifndef GAMEENTITIES_ANIMATORS_ANIMATORHANDLER_H_
#define GAMEENTITIES_ANIMATORS_ANIMATORHANDLER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <any>

//Other libraries headers

//Own components headers
#include "PathAnimator.h"
#include "ScaleAnimator.h"
#include "SpeechAnimator.h"
#include "gameentities/proxies/AnimatorHandlerProxyInterface.hpp"

//Forward declarations
class GameProxyInterface;
class GridContainerProxyInterface;

enum class AnimEvent {
  SET_SCALE_AMIM_START_TARGET,
  SET_SCALE_AMIM_END_TARGET,
  LOAD_ANIM_PATH,
  START_PATH_ANIM,
  START_NO_PATH_ANIM
};

class AnimatorHandler: public AnimatorHandlerProxyInterface {
public:
  AnimatorHandler();
  virtual ~AnimatorHandler() = default;

  int32_t init(GameProxyInterface *gameInterface,
               GridContainerProxyInterface *gridInterface);

  void draw();

  bool isActive() const;

  void perform(const AnimEvent event, const std::any &args = std::any { });

private:
  enum class ScaleAnimTargetType {
    START_TARGET, END_TARGET
  };

  virtual void onAnimFinished(const AnimType type) override final;

  void performSetScaleAnimTarget(const ScaleAnimTargetType type,
                                 const std::any &args);
  void performLoadAnimPath(const std::any &args);

  PathAnimator _pathAnimator;

  ScaleAnimator _scaleAnimator;

  SpeechAnimator _speechAnimator;

  GameProxyInterface *_gameInterface;
};

#endif /* GAMEENTITIES_ANIMATORS_ANIMATORHANDLER_H_ */
