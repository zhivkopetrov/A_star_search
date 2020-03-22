#ifndef PATHFINDING_NODE_H_
#define PATHFINDING_NODE_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/drawing/Point.h"

//Forward declarations

struct Node {
  Node(const Point &inputPosition, Node *inputParent = nullptr)
      : parent(inputParent), position(inputPosition), G(0), H(0) {
  }

  inline int32_t getScore() const {
    return G + H;
  }

  Node *parent;
  Point position;
  int32_t G;
  int32_t H;
};

#endif /* PATHFINDING_NODE_H_ */
