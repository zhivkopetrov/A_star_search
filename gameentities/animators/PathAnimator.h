#ifndef GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_
#define GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "utils/drawing/Point.h"
#include "utils/drawing/Image.h"
#include "utils/time/TimerClient.h"

//Forward declarations
class GridContainerProxyInterface;
class AnimatorHandlerProxyInterface;

class PathAnimator: public TimerClient {
public:
  PathAnimator();
  ~PathAnimator() = default;

  int32_t init(AnimatorHandlerProxyInterface *animatorHandlerInterface,
               GridContainerProxyInterface *gridInterface,
               const int32_t batmanRsrcId, const int32_t pathTimerId);

  void loadPath(std::vector<Point> &path);

  void draw();

  void activateAnim();

  inline bool isActive() const {
    return _isActive;
  }

private:
  void onTimeout(const int32_t timerId) final;

  void processAnim();

  void moveImage();

  void loadNextOffsets();

  enum InternalDefines {
    ANIM_MOVES = 10
  };

  AnimatorHandlerProxyInterface *_animatorHandlerInterface;
  GridContainerProxyInterface *_gridInterface;

  std::vector<Point> _pathToAnimate;

  Image _batmanImg;

  int32_t _animStep;

  int32_t _offsetX;
  int32_t _offsetY;

  int32_t _pathTimerId;

  bool _isActive;
};

#endif /* GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_ */
