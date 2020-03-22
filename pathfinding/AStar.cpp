//Corresponding header
#include "AStar.h"

//C system headers

//C++ system headers
#include <algorithm>
#include <cmath>

//Other libraries headers

//Own components headers
#include "Node.h"
#include "common/CommonDefines.h"
#include "utils/Log.h"

AStar::AStar()
    : _walls(nullptr), _allowedDirectionsCount(0), _mazeWidth(0), _mazeHeight(0) {

}

int32_t AStar::init(const int32_t mazeWidth, const int32_t mazeHeight,
                    const bool isDiagonalMovementEnabled,
                    HeuristicFunction heuristic,
                    const std::vector<Point> *walls) {
  int32_t err = EXIT_SUCCESS;

  _mazeWidth = mazeWidth;
  _mazeHeight = mazeHeight;
  _heuristic = heuristic;
  _walls = walls;

  _allowedDirectionsCount =
      (isDiagonalMovementEnabled) ? DIAGONAL_MOVEMENTS : NON_DIAGONAL_MOVEMENTS;

  _moveDirections = { { 0, 1 }, //Right
      { 1, 0 }, //Left
      { 0, -1 }, //UP
      { -1, 0 }, //DOWN
      { -1, -1 }, //UP_LEFT
      { 1, 1 }, //DOWN_RIGHT
      { -1, 1 }, //UP_RIGHT
      { 1, -1 }  //DOWN_LEFT
  };

  return err;
}

std::vector<Point> AStar::findPath(const Point &source, const Point &target) {
  std::vector<Point> path;
  if (detectCollision(source) || detectCollision(target)) {
    LOGERR("Error, source or target are out of bounds");
    return path;
  }

  Node *current = nullptr;
  std::set<Node*> openSet;
  std::set<Node*> closedSet;
  openSet.insert(new Node(source));

  while (!openSet.empty()) {
    current = *openSet.begin();
    for (auto node : openSet) {
      //update the score of the node if it's lower
      if (node->getScore() <= current->getScore()) {
        current = node;
      }
    }

    //destination found -> break the loop
    if (current->position == target) {
      break;
    }

    closedSet.insert(current);
    openSet.erase(std::find(openSet.begin(), openSet.end(), current));

    //iterate over the allowed directions
    for (uint32_t i = 0; i < _allowedDirectionsCount; ++i) {
      Point newCoordinates(current->position + _moveDirections[i]);
      if (detectCollision(newCoordinates) || findNodeOnList(closedSet,
              newCoordinates)) {
        //if node is already visited or is out of bound -> skip it
        continue;
      }

      const int32_t totalCost = current->G
          + ( (i < NON_DIAGONAL_MOVEMENTS) ?
              Movement::NON_DIAGONAL_DISTANCE : Movement::DIAGONAL_DISTANCE);

      Node *successor = findNodeOnList(openSet, newCoordinates);
      if (successor == nullptr) {
        successor = new Node(newCoordinates, current);
        successor->G = totalCost;
        successor->H = _heuristic(successor->position, target);
        openSet.insert(successor);
      } else if (totalCost < successor->G) {
        successor->parent = current;
        successor->G = totalCost;
      }
    }
  }

  //build the path
  while (current != nullptr) {
    path.emplace_back(current->position);
    current = current->parent;
  }

  releaseNodes(openSet);
  releaseNodes(closedSet);

  return path;
}

Node* AStar::findNodeOnList(const std::set<Node*> &nodes,
                            const Point &position) {
  for (auto node : nodes) {
    if (node->position == position) {
      return node;
    }
  }
  return nullptr;
}

void AStar::releaseNodes(std::set<Node*> &nodes) {
  for (auto it = nodes.begin(); it != nodes.end();) {
    delete *it;
    it = nodes.erase(it);
  }
}

bool AStar::detectCollision(const Point &position) {
  if (position.x < 0 || position.x >= _mazeWidth
      || position.y < 0
      || position.y >= _mazeHeight
      || std::find( (*_walls).begin(), (*_walls).end(), position) != (*_walls).end()) {
    return true;
  }

  return false;
}

