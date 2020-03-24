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

  void clearGridFromAStarPathNodes();

  void addCollision(const int32_t nodeX, const int32_t nodeY);

  void removeCollision(const int32_t nodeX, const int32_t nodeY);

  void addStartNode(const int32_t nodeX, const int32_t nodeY);

  void addEndNode(const int32_t nodeX, const int32_t nodeY);

  virtual Point getNodeCoordinates(const int32_t nodeX,
                                   const int32_t nodeY) const override final;

private:
  virtual void addAStarPathNode(const int32_t nodeX, const int32_t nodeY)
      override final;

  void onWallAdd();

  void onWallRemove();

  void onStartNodeEntered();

  void onEndNodeEntered();

  //returns false if event has not selected any node
  bool getSelectedNode(int32_t *nodeX, int32_t *nodeY);

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

