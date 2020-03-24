//Corresponding header
#include "GridContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "proxies/GameProxyInterface.hpp"

GridContainer::GridContainer()
    : _gameInterface(nullptr), _pathNodeRsrcId(0), _wallNodeRsrcId(0),
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
  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      _pathNodes[i][j].setTextureId(_pathNodeRsrcId);
      _pathNodes[i][j].hide();
    }
  }
}

void GridContainer::clearGridFromAStarPathNodes() {
  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      if (_pathNodeRsrcId == _pathNodes[i][j].getRsrcId()) {
        _pathNodes[i][j].hide();
      }
    }
  }
}

void GridContainer::addCollision(const int32_t nodeX, const int32_t nodeY) {
  _pathNodes[nodeY][nodeX].setTextureId(_wallNodeRsrcId);
  _pathNodes[nodeY][nodeX].show();
}

void GridContainer::removeCollision(const int32_t nodeX, const int32_t nodeY) {
  _pathNodes[nodeY][nodeX].hide();
}

void GridContainer::addStartNode(const int32_t nodeX, const int32_t nodeY) {
  //first clear the old start node, if any
  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      if (_startNodeRsrcId == _pathNodes[i][j].getRsrcId()) {
        _pathNodes[i][j].hide();
        break;
      }
    }
  }

  _pathNodes[nodeY][nodeX].setTextureId(_startNodeRsrcId);
  _pathNodes[nodeY][nodeX].show();
}

void GridContainer::addEndNode(const int32_t nodeX, const int32_t nodeY) {
  //first clear the old start node, if any
  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      if (_endNodeRsrcId == _pathNodes[i][j].getRsrcId()) {
        _pathNodes[i][j].hide();
      }
    }
  }

  _pathNodes[nodeY][nodeX].setTextureId(_endNodeRsrcId);
  _pathNodes[nodeY][nodeX].show();
}

Point GridContainer::getNodeCoordinates(const int32_t nodeX,
                                        const int32_t nodeY) const {
  return _pathNodes[nodeY][nodeX].getPosition();
}

void GridContainer::addAStarPathNode(const int32_t nodeX, const int32_t nodeY) {
  _pathNodes[nodeY][nodeX].setTextureId(_pathNodeRsrcId);
  _pathNodes[nodeY][nodeX].show();
}

void GridContainer::onWallAdd() {
  int32_t nodeX = 0;
  int32_t nodeY = 0;

  if (getSelectedNode(&nodeX, &nodeY)) {
    addCollision(nodeX, nodeY);
    _gameInterface->onNodeChanged(NodeType::WALL_ADD, nodeX, nodeY);
  }
}

void GridContainer::onWallRemove() {
  int32_t nodeX = 0;
  int32_t nodeY = 0;

  if (getSelectedNode(&nodeX, &nodeY)) {
    removeCollision(nodeX, nodeY);
    _gameInterface->onNodeChanged(NodeType::WALL_REMOVE, nodeX, nodeY);
  }
}

void GridContainer::onStartNodeEntered() {
  int32_t nodeX = 0;
  int32_t nodeY = 0;

  if (getSelectedNode(&nodeX, &nodeY)) {
    _gameInterface->onNodeChanged(NodeType::START_CHANGE, nodeX, nodeY);
  }
}

void GridContainer::onEndNodeEntered() {
  int32_t nodeX = 0;
  int32_t nodeY = 0;

  if (getSelectedNode(&nodeX, &nodeY)) {
    _gameInterface->onNodeChanged(NodeType::END_CHANGE, nodeX, nodeY);
  }
}

bool GridContainer::getSelectedNode(int32_t *nodeX, int32_t *nodeY) {
  SDL_Point mousePos;
  /* capture mouse position on the screen */
  SDL_GetMouseState(&mousePos.x, &mousePos.y);

  SDL_Rect currBoundaryRect;
  currBoundaryRect.w = Grid::TILE_DIMENSION;
  currBoundaryRect.h = Grid::TILE_DIMENSION;

  Point currNodePos;

  for (int32_t i = 0; i < Grid::GRID_HEIGHT; ++i) {
    for (int32_t j = 0; j < Grid::GRID_WIDTH; ++j) {
      currNodePos = _pathNodes[i][j].getPosition();
      currBoundaryRect.x = currNodePos.x;
      currBoundaryRect.y = currNodePos.y;

      if (SDL_PointInRect(&mousePos, &currBoundaryRect)) {
        *nodeX = j;
        *nodeY = i;

        return true;
      }
    }
  }

  return false;
}

