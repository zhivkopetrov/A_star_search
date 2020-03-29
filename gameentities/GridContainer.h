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

//Forward declarations
class GameProxyInterface;
struct Point;
class InputEvent;

class GridContainer: public GridContainerProxyInterface {
public:
  GridContainer();

  virtual ~GridContainer() = default;

  int32_t init(GameProxyInterface *gameInterface,
               const int32_t vericalLineRsrcId,
               const int32_t horizontalLineRsrcId,
               const int32_t startNodeRsrcId, const int32_t endNodeRsrcId,
               const int32_t pathNodeRsrcId, const int32_t wallNodeRsrcId,
               const int32_t predefinedObstacleRsrcId);

  void draw();

  void handleEvent(const InputEvent &e);

  void clearGrid();

  bool isReadyToEvaluate();

  virtual Point getNodeCoordinates(const Point &nodePos) const override final;

  inline Point getStartNodePos() const {
    return _startNodePos;
  }

  inline Point getEndNodePos() const {
    return _endNodePos;
  }

private:
  virtual void addPathNode(const Point &nodePos) override final;

  virtual void placePredefinedObstacles(const std::vector<Obstacle> &obstacles)
      override final;
  virtual void removePredefinedObstacles(const std::vector<Obstacle> &obstacles)
      override final;

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
};

#endif /* GAMEENTITIES_GRIDCONTAINER_H_ */

