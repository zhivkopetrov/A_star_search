#ifndef GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_
#define GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "utils/drawing/Point.h"
#include "utils/time/TimerClient.h"

//Forward declarations
class GridContainer;

class PathAnimator : public TimerClient {
public:
  PathAnimator();
  ~PathAnimator() = default;

  int32_t init(GridContainer *gridContainer, const int32_t pathTimerId);

  void startAnimation(std::vector<Point> &path);

  void update(const int32_t elapsedTime);

  void draw();

  inline bool isAnimActive() const {
    return _isActive;
  }

private:
  virtual void onTimeout(const int32_t timerId) override final;

  void processAnim();

  GridContainer *_gridContainer;

  std::vector<Point> _pathToAnimate;

  Point _batmanPos;

  int32_t _pathTimerId;

  bool _isActive;
};

#endif /* GAMEENTITIES_ANIMATORS_PATHANIMATOR_H_ */
