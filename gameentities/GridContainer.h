#ifndef GAMEENTITIES_GRIDCONTAINER_H_
#define GAMEENTITIES_GRIDCONTAINER_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "proxies/GridContainerProxyInterface.hpp"
#include "utils/drawing/DrawParams.h"
#include "utils/drawing/Image.h"
#include "utils/drawing/FBO.h"

//Forward declarations
class GameProxyInterface;
struct Point;
class InputEvent;

struct GridContainerCfg {
  GridContainerCfg(GameProxyInterface *interface,
                   const int32_t inputVerticalLineRsrcId,
                   const int32_t inputHorizontalLineRsrcId,
                   const int32_t inputStartNodeRsrcId,
                   const int32_t inputEndNodeRsrcId,
                   const int32_t inputPathNodeRsrcId,
                   const int32_t inputWallNodeRsrcId,
                   const int32_t inputPredefinedObstacleRsrcId)
      : gameInterface(interface), verticalLineRsrcId(inputVerticalLineRsrcId),
        horizontalLineRsrcId(inputHorizontalLineRsrcId),
        startNodeRsrcId(inputStartNodeRsrcId),
        endNodeRsrcId(inputEndNodeRsrcId), pathNodeRsrcId(inputPathNodeRsrcId),
        wallNodeRsrcId(inputWallNodeRsrcId),
        predefinedObstacleRsrcId(inputPredefinedObstacleRsrcId) {

  }
  GridContainerCfg() = delete;

  GameProxyInterface *gameInterface;
  const int32_t verticalLineRsrcId;
  const int32_t horizontalLineRsrcId;
  const int32_t startNodeRsrcId;
  const int32_t endNodeRsrcId;
  const int32_t pathNodeRsrcId;
  const int32_t wallNodeRsrcId;
  const int32_t predefinedObstacleRsrcId;
};

class GridContainer: public GridContainerProxyInterface {
public:
  GridContainer();

  int32_t init(const GridContainerCfg &cfg);

  void draw();

  void handleEvent(const InputEvent &e);

  void clearGrid();

  bool isReadyToEvaluate();

  Point getNodeCoordinates(const Point &nodePos) const final;

  inline Point getStartNodePos() const {
    return _startNodePos;
  }

  inline Point getEndNodePos() const {
    return _endNodePos;
  }

private:
  void addPathNode(const Point &nodePos) final;

  void placePredefinedObstacles(const std::vector<Obstacle> &obstacles) final;

  void removePredefinedObstacles(const std::vector<Obstacle> &obstacles) final;

  void addCollision(const Point &nodePos);

  void removeNode(const Point &nodePos);

  void addStartNode(const Point &nodePos);

  void addEndNode(const Point &nodePos);

  void onWallAdd(const InputEvent &e);

  void onWallRemove(const InputEvent &e);

  void onStartNodeEntered(const InputEvent &e);

  void onEndNodeEntered(const InputEvent &e);

  //returns false if event has not selected any node
  bool getSelectedNode(const InputEvent &e, Point &outNodePos);

  void createGridLineFBO();

  enum InternalDefines {
    TOTAL_LINES_COUNT = Grid::GRID_HEIGHT + Grid::GRID_WIDTH
  };

  GameProxyInterface *_gameInterface;

  Point _startNodePos;
  Point _endNodePos;
  int32_t _pathNodeRsrcId;
  int32_t _wallNodeRsrcId;
  int32_t _startNodeRsrcId;
  int32_t _endNodeRsrcId;
  int32_t _predefinedObstacleRsrcId;

  Image _gridLines[TOTAL_LINES_COUNT];
  Image _pathNodes[Grid::GRID_HEIGHT][Grid::GRID_WIDTH];

  FBO _gridLinesFBO;
};

#endif /* GAMEENTITIES_GRIDCONTAINER_H_ */

