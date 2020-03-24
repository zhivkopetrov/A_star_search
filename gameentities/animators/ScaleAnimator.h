#ifndef GAMEENTITIES_ANIMATORS_SCALEANIMATOR_H_
#define GAMEENTITIES_ANIMATORS_SCALEANIMATOR_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/drawing/Image.h"
#include "utils/time/TimerClient.h"

//Forward declarations
class PathAnimatorProxyInterface;

class ScaleAnimator : public TimerClient {
public:
  ScaleAnimator();
  ~ScaleAnimator() = default;

  int32_t init(PathAnimatorProxyInterface *pathAnimInterface,
               const uint8_t batmanRsrcId,
               const int32_t scaleTimerId);

  void draw();

  void setTargetPos(const Point& pos);

  void startAnim();

  inline bool isActive() const {
    return _isActive;
  }

private:
  enum InternalDefines {
    TOTAL_ANIM_STEPS = 100
  };

  virtual void onTimeout(const int32_t timerId) override final;

  void processAnim();

  void moveImage();
  void scaleImage();

  void calculateAnimInternals();

  Image _batman;

  PathAnimatorProxyInterface *_pathAnimInterface;

  Point _endPos;
  Point _origStartPos;

  int32_t _imgOrigWidth;
  int32_t _imgOrigHeight;

  int32_t _remainingSteps;
  int32_t _xOffset;
  int32_t _yOffset;
  int32_t _remainderX;
  int32_t _remainderY;

  int32_t _scaleXOffset;
  int32_t _scaleYOffset;
  int32_t _remainderScaleX;
  int32_t _remainderScaleY;

  int32_t _scaleTimerId;

  bool _isActive;
};

#endif /* GAMEENTITIES_ANIMATORS_SCALEANIMATOR_H_ */
