#ifndef PATHFINDING_HEURISTIC_H_
#define PATHFINDING_HEURISTIC_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <functional>

//Other libraries headers

//Own components headers

//Forward declarations
struct Point;

using HeuristicFunction = std::function<int32_t(const Point &, const Point &)>;

class Heuristic {
public:
  static int32_t manhattan(const Point &source, const Point &target);
  static int32_t diagonal(const Point &source, const Point &target);
  static int32_t euclidean(const Point &source, const Point &target);

  static Point getDelta(const Point &source, const Point &target);
};

#endif /* PATHFINDING_HEURISTIC_H_ */
