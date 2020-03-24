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
#include "GeneralTextAnimator.h"

//Forward declarations

enum class AnimEvent {
  SET_SCALE_AMIM_TARGET,
  LOAD_ANIM_PATH,
  START_PATH_ANIM,
  START_NO_PATH_ANIM
};

class AnimatorHandler {
public:
  AnimatorHandler() = default;
  ~AnimatorHandler() = default;

  int32_t init(GridContainerProxyInterface *gridInterface);

  void draw();

  bool isActive() const;

  void perform(const AnimEvent event, const std::any & args = std::any{});

private:
  void performSetScaleAnimTarget(const std::any & args);
  void performLoadAnimPath(const std::any & args);

  PathAnimator _pathAnimator;

  ScaleAnimator _scaleAnimator;

  GeneralTextAnimator _generalTextAnimator;
};

#endif /* GAMEENTITIES_ANIMATORS_ANIMATORHANDLER_H_ */
