#ifndef GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_
#define GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "gameentities/proxies/PathAnimatorProxyInterface.hpp"
#include "utils/drawing/Point.h"
#include "utils/drawing/Image.h"
#include "utils/time/TimerClient.h"

//Forward declarations
class GridContainerProxyInterface;

class PathAnimator: public TimerClient,
                    public PathAnimatorProxyInterface {
public:
  PathAnimator();
  ~PathAnimator() = default;

  int32_t init(GridContainerProxyInterface *gridInterface,
               const uint8_t batmanRsrcId, const int32_t pathTimerId);

  void loadPath(std::vector<Point> &path);

  void draw();

  inline bool isActive() const {
    return _isActive;
  }

private:
  virtual void onScaleAnimFinished() override final;

  virtual void onTimeout(const int32_t timerId) override final;

  void processAnim();

  GridContainerProxyInterface *_gridInterface;

  std::vector<Point> _pathToAnimate;

  Image _batmanImg;

  int32_t _pathTimerId;

  bool _isActive;
};

#endif /* GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_ */
