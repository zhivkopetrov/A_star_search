/*
 * Point.h
 *
 *  Created on: Jul 20, 2017
 *      Author: Zhivko Petrov
 */

#ifndef UTILS_DRAWING_POINT_H_
#define UTILS_DRAWING_POINT_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations

struct Point {
  static const Point ZERO;
  static const Point UNDEFINED;

  Point();
  Point(const int32_t inputX, const int32_t inputY);

  inline bool operator ==(const Point &other) const {
    return (x == other.x && y == other.y);
  }

  inline bool operator !=(const Point &other) const {
    return ! (*this == other);
  }

  inline Point operator+(const Point &other) const {
    return {x + other.x, y + other.y};
  }

  int32_t x;
  int32_t y;
};

#endif /* UTILS_DRAWING_POINT_H_ */
