//Corresponding header
#include "GridContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "proxies/GameProxyInterface.hpp"
#include "utils/drawing/Rectangle.h"

GridContainer::GridContainer()
    : _gameInterface(nullptr), _startNodePos(Point::UNDEFINED),
      _endNodePos(Point::UNDEFINED), _pathNodeRsrcId(0), _wallNodeRsrcId(0),
      _startNodeRsrcId(0), _endNodeRsrcId(0) {

}

int32_t GridContainer::init(GameProxyInterface *gameInterface,
                            const uint8_t vericalLineRsrcId,
                            const uint8_t horizontalLineRsrcId,
                            const uint8_t startNodeRsrcId,
                            const uint8_t endNodeRsrcId,
                            const uint8_t pathNodeRsrcId,
                            const uint8_t wallNodeRsrcId) {
  _gameInterface = gameInterface;
  _pathNodeRsrcId = pathNodeRsrcId;
  _wallNodeRsrcId = wallNodeRsrcId;
  _startNodeRsrcId = startNodeRsrcId;
  _endNodeRsrcId = endNodeRsrcId;

  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    _gridLines[i].create(horizontalLineRsrcId);
    _gridLines[i].setPosition(0, i * Grid::TILE_OFFSET);
  }

  for (int32_t i = 0; i < Grid::GRID_WIDTH; ++i) {
    _gridLines[i + Grid::GRID_HEIGHT].create(vericalLineRsrcId);
    _gridLines[i + Grid::GRID_HEIGHT].setPosition(i * Grid::TILE_OFFSET, 0);
  }

  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      _pathNodes[i][j].create(pathNodeRsrcId);
      _pathNodes[i][j].setPosition(
          ( (j * Grid::TILE_OFFSET) + Grid::LINE_OFFSET),
          ( (i * Grid::TILE_OFFSET) + Grid::LINE_OFFSET));
    }
  }

  clearGrid();

  return EXIT_SUCCESS;
}

void GridContainer::draw() {
  for (int32_t i = 0; i < TOTAL_LINES_COUNT; ++i) {
    _gridLines[i].draw();
  }

  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      _pathNodes[i][j].draw();
    }
  }
}

void GridContainer::handleUserEvent(const SDL_Event &e) {
  if (e.type == SDL_KEYUP) {
    switch (e.key.keysym.sym) {
    case SDLK_s:
      onStartNodeEntered();
      break;

    case SDLK_w:
      onWallAdd();
      break;

    case SDLK_r:
      onWallRemove();
      break;

    case SDLK_e:
      onEndNodeEntered();
      break;

    default:
      break;
    }
  } else if (e.type == SDL_MOUSEBUTTONDOWN) {
    if (e.button.button == SDL_BUTTON_LEFT) {
      onWallAdd();
    } else if (e.button.button == SDL_BUTTON_RIGHT) {
      onWallRemove();
    }
  }
}

void GridContainer::clearGrid() {
  _startNodePos = Point::UNDEFINED;
  _endNodePos = Point::UNDEFINED;

  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      _pathNodes[i][j].setTextureId(_pathNodeRsrcId);
      _pathNodes[i][j].hide();
    }
  }
}

bool GridContainer::isReadyToEvaluate() {
  return (Point::UNDEFINED != _startNodePos)
      && (Point::UNDEFINED != _endNodePos);
}

void GridContainer::addAStarPathNode(const Point &nodePos) {
  _pathNodes[nodePos.y][nodePos.x].setTextureId(_pathNodeRsrcId);
  _pathNodes[nodePos.y][nodePos.x].show();
}

void GridContainer::addCollision(const Point &nodePos) {
  if (_startNodePos == nodePos) {
    _startNodePos = Point::UNDEFINED;
  } else if (_endNodePos == nodePos) {
    _endNodePos = Point::UNDEFINED;
  }

  _pathNodes[nodePos.y][nodePos.x].setTextureId(_wallNodeRsrcId);
  _pathNodes[nodePos.y][nodePos.x].show();
}

void GridContainer::removeNode(const Point &nodePos) {
  _pathNodes[nodePos.y][nodePos.x].hide();
}

void GridContainer::addStartNode(const Point &nodePos) {
  //first clear the old start node, if any
  if (Point::UNDEFINED != _startNodePos) {
    _pathNodes[_startNodePos.y][_startNodePos.x].hide();
  }
  if (_endNodePos == nodePos) {
    _endNodePos = Point::UNDEFINED;
  }

  _startNodePos = nodePos;
  _pathNodes[nodePos.y][nodePos.x].setTextureId(_startNodeRsrcId);
  _pathNodes[nodePos.y][nodePos.x].show();
}

void GridContainer::addEndNode(const Point &nodePos) {
  //first clear the old start node, if any
  if (Point::UNDEFINED != _endNodePos) {
    _pathNodes[_endNodePos.y][_endNodePos.x].hide();
  }
  if (_startNodePos == nodePos) {
    _startNodePos = Point::UNDEFINED;
  }

  _endNodePos = nodePos;
  _pathNodes[nodePos.y][nodePos.x].setTextureId(_endNodeRsrcId);
  _pathNodes[nodePos.y][nodePos.x].show();
}

Point GridContainer::getNodeCoordinates(const Point &nodePos) const {
  return _pathNodes[nodePos.y][nodePos.x].getPosition();
}

void GridContainer::onWallAdd() {
  Point nodePos;
  if (getSelectedNode(nodePos)) {
    addCollision(nodePos);
    _gameInterface->onNodeChanged(NodeType::WALL_ADD, nodePos);
  }
}

void GridContainer::onWallRemove() {
  Point nodePos;
  if (getSelectedNode(nodePos)) {
    removeNode(nodePos);
    _gameInterface->onNodeChanged(NodeType::NODE_REMOVE, nodePos);
  }
}

void GridContainer::onStartNodeEntered() {
  Point nodePos;
  if (getSelectedNode(nodePos)) {
    addStartNode(nodePos);
    _gameInterface->onNodeChanged(NodeType::START_CHANGE, nodePos);
  }
}

void GridContainer::onEndNodeEntered() {
  Point nodePos;
  if (getSelectedNode(nodePos)) {
    addEndNode(nodePos);
    _gameInterface->onNodeChanged(NodeType::END_CHANGE, nodePos);
  }
}

bool GridContainer::getSelectedNode(Point &nodePos) {
  Point mousePos;
  /* capture mouse position on the screen */
  SDL_GetMouseState(&mousePos.x, &mousePos.y);

  Rectangle currBoundaryRect;
  currBoundaryRect.w = Grid::TILE_DIMENSION;
  currBoundaryRect.h = Grid::TILE_DIMENSION;

  Point currNodePos;

  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      currNodePos = _pathNodes[i][j].getPosition();
      currBoundaryRect.x = currNodePos.x;
      currBoundaryRect.y = currNodePos.y;

      if (Rectangle::isPointInRect(mousePos, currBoundaryRect)) {
        nodePos.x = j;
        nodePos.y = i;

        return true;
      }
    }
  }

  return false;
}

