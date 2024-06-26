#include "Heuristic.h"
//Corresponding header

//C system headers

//C++ system headers
#include <cmath>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "utils/drawing/Point.h"

static constexpr double DIAG_DISTANCE_DOUBLE = static_cast<double>(Movement::NON_DIAGONAL_DISTANCE);

Point Heuristic::getDelta(const Point &source, const Point &target) {
  return {abs(source.x - target.x), abs(source.y - target.y)};
}

int32_t Heuristic::manhattan(const Point &source, const Point &target) {
  const Point delta = getDelta(source, target);
  return (Movement::NON_DIAGONAL_DISTANCE * (delta.x + delta.y));
}

int32_t Heuristic::diagonal(const Point &source, const Point &target) {
  const Point delta = getDelta(source, target);
  return (Movement::NON_DIAGONAL_DISTANCE * (delta.x + delta.y)) +
      (Movement::DIAGONAL_COMPENSATION * std::min(delta.x, delta.y));
}

int32_t Heuristic::euclidean(const Point &source, const Point &target) {
  const Point delta = getDelta(source, target);
  return static_cast<int32_t>((DIAG_DISTANCE_DOUBLE *
      sqrt( (delta.x * delta.x) + (delta.y * delta.y))));
}

