#include "Heuristic.h"
//Corresponding header

//C system headers

//C++ system headers
#include <cmath>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "utils/drawing/Point.h"

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
  return (Movement::NON_DIAGONAL_DISTANCE *
      sqrt( (delta.x * delta.x) + (delta.y * delta.y)));
}

