#ifndef GAMEENTITIES_ANIMATORS_MENUMOVEANIMATOR_H_
#define GAMEENTITIES_ANIMATORS_MENUMOVEANIMATOR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "utils/drawing/FBO.h"
#include "utils/time/TimerClient.h"

//Forward declarations
class AnimatorHandlerProxyInterface;

class MenuMoveAnimator : public TimerClient {
public:
  MenuMoveAnimator();

  int32_t init(AnimatorHandlerProxyInterface *interface,
               const Rectangle &animFBODimensions,
               const int32_t moveTimerId);

  void startAnim(const OptionAnimStatus type);

  void draw();

  void updateAnimContent(const std::vector<const Widget*> &widgets);

  inline bool isActive() const {
    return _isActive;
  }

private:
  void onTimeout(const int32_t timerId) final;

  void processAnim();

  void updateAnimInternals();

  enum InternalDefines {
    ANIM_MOVE_STEP = 10,
    MAX_ANIM_STEPS = OptionMenuDimensions::MENU_OFFSET_X / ANIM_MOVE_STEP,
    ANIM_OPACITY_STEP = 5
  };

  FBO _animContent;

  AnimatorHandlerProxyInterface *_animatorHandlerInterface;

  int32_t _moveTimerId;

  int32_t _animStepsLeft;

  int32_t _updateSign;

  int32_t _currAnimOpacity;

  OptionAnimStatus _currAnimStatus;

  bool _isActive;
};

#endif /* GAMEENTITIES_ANIMATORS_MENUMOVEANIMATOR_H_ */
