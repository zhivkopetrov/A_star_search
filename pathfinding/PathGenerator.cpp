//Corresponding header
#include "PathGenerator.h"

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "Node.h"
#include "common/CommonDefines.h"
#include "gameentities/proxies/ObstacleHandlerProxyInterface.hpp"
#include "utils/Log.h"

PathGenerator::PathGenerator()
    : _obstacleHandlerInterface(nullptr), _allowedDirectionsCount(0),
      _mazeWidth(0), _mazeHeight(0) {

}

int32_t PathGenerator::init(
    const int32_t mazeWidth, const int32_t mazeHeight,
    ObstacleHandlerProxyInterface *obstacleHandlerInterface) {
  _mazeWidth = mazeWidth;
  _mazeHeight = mazeHeight;
  _obstacleHandlerInterface = obstacleHandlerInterface;

  _moveDirections = { { 0, 1 }, //Right
      { 1, 0 }, //Left
      { 0, -1 }, //UP
      { -1, 0 }, //DOWN
      { -1, -1 }, //UP_LEFT
      { 1, 1 }, //DOWN_RIGHT
      { -1, 1 }, //UP_RIGHT
      { 1, -1 }  //DOWN_LEFT
  };

  setHeuristic(false);

  return EXIT_SUCCESS;
}

std::vector<Point> PathGenerator::findPath(const Point &source,
                                           const Point &target) const {
  std::vector<Point> path;
  if (detectCollision(source) || detectCollision(target)) {
    LOGERR("Error, source or target are out of bounds");
    return path;
  }

  std::shared_ptr<Node> current;
  std::set<std::shared_ptr<Node>> openSet;
  std::set<std::shared_ptr<Node>> closedSet;
  openSet.insert(std::make_shared<Node>(source));

  while (!openSet.empty()) {
    current = *openSet.begin();
    for (const auto &node : openSet) {
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
      if (detectCollision(newCoordinates) ||
          findNodeOnList(closedSet, newCoordinates)) {
        //if node is already visited or is out of bound -> skip it
        continue;
      }

      const int32_t totalCost = current->G
          + ( (i < NON_DIAGONAL_MOVEMENTS) ?
              Movement::NON_DIAGONAL_DISTANCE : Movement::DIAGONAL_DISTANCE);

      std::shared_ptr<Node> successor = findNodeOnList(openSet, newCoordinates);
      if (successor == nullptr) {
        successor = std::make_shared<Node>(newCoordinates, current.get());
        successor->G = totalCost;
        successor->H = _heuristic(successor->position, target);
        openSet.insert(successor);
      } else if (totalCost < successor->G) {
        successor->parent = current.get();
        successor->G = totalCost;
      }
    }
  }

  Node *pathNode = current.get();
  //build the path
  while (pathNode != nullptr) {
    path.emplace_back(pathNode->position);
    pathNode = pathNode->parent;
  }

  return path;
}

void PathGenerator::changeDiagonalOption(const std::any &value) {
  try {
    const auto isDiagonalMovementAllowed = std::any_cast<bool>(value);
    setHeuristic(isDiagonalMovementAllowed);
  } catch (const std::bad_any_cast &e) {
    LOGERR("any_cast throwed: %s", e.what());
  }
}

void PathGenerator::setHeuristic(const bool isDiagonalMovementAllowed) {
  _heuristic = Heuristic::manhattan;

  //diagonal heuristic is better for diagonal movements
  if (isDiagonalMovementAllowed) {
    _heuristic = Heuristic::diagonal;
  }

  _allowedDirectionsCount =
      (isDiagonalMovementAllowed) ? DIAGONAL_MOVEMENTS : NON_DIAGONAL_MOVEMENTS;
}

std::shared_ptr<Node> PathGenerator::findNodeOnList(
    const std::set<std::shared_ptr<Node>> &nodes, const Point &position) const {
  for (const auto &node : nodes) {
    if (node->position == position) {
      return node;
    }
  }
  return nullptr;
}

bool PathGenerator::detectCollision(const Point &position) const {
  if (position.x < 0 || position.x >= _mazeWidth || position.y < 0
      || position.y >= _mazeHeight
      || _obstacleHandlerInterface->isIntersectingObstacle(position)) {
    return true;
  }

  return false;
}
