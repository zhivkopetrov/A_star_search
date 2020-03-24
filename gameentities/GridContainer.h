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
union SDL_Event;

class GridContainer: public GridContainerProxyInterface {
public:
  GridContainer();

  virtual ~GridContainer() = default;

  int32_t init(GameProxyInterface *gameInterface,
               const uint8_t vericalLineRsrcId,
               const uint8_t horizontalLineRsrcId,
               const uint8_t startNodeRsrcId, const uint8_t endNodeRsrcId,
               const uint8_t pathNodeRsrcId, const uint8_t wallNodeRsrcId);

  void draw();

  void handleUserEvent(const SDL_Event &e);

  void clearGrid();

  virtual Point getNodeCoordinates(const Point& nodePos) const override final;

private:
  virtual void addAStarPathNode(const Point& nodePos) override final;

  void clearGridFromAStarPathNodes();

  void addCollision(const Point &nodePos);

  void removeNode(const Point &nodePos);

  void addStartNode(const Point &nodePos);

  void addEndNode(const Point &nodePos);

  void onWallAdd();

  void onWallRemove();

  void onStartNodeEntered();

  void onEndNodeEntered();

  //returns false if event has not selected any node
  bool getSelectedNode(Point &nodePos);

  enum InternalDefines {
    TOTAL_LINES_COUNT = Grid::GRID_HEIGHT + Grid::GRID_WIDTH
  };

  GameProxyInterface *_gameInterface;

  uint8_t _pathNodeRsrcId;
  uint8_t _wallNodeRsrcId;
  uint8_t _startNodeRsrcId;
  uint8_t _endNodeRsrcId;

  Image _gridLines[TOTAL_LINES_COUNT];
  Image _pathNodes[Grid::GRID_HEIGHT][Grid::GRID_WIDTH];
};

#endif /* GAMEENTITIES_GRIDCONTAINER_H_ */

