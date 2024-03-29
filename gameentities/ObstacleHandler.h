#ifndef GAMEENTITIES_OBSTACLEHANDLER_H_
#define GAMEENTITIES_OBSTACLEHANDLER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>
#include <string>
#include <any>

//Other libraries headers

//Own components headers
#include "proxies/ObstacleHandlerProxyInterface.hpp"
#include "proxies/GridContainerProxyInterface.hpp"

//Forward declarations

class ObstacleHandler: public ObstacleHandlerProxyInterface {
public:
  ObstacleHandler();

  int32_t init(GridContainerProxyInterface *gridInterface,
               const std::string &levelsFolderPath, const int32_t levelsCount);

  void changeLevel(const std::any &value);

  inline void addObstacle(const Point &nodePos) {
    _addedObstacles.push_back(nodePos);
  }

  inline void clearAddedObstacles() {
    _addedObstacles.clear();
  }

  int32_t getCurrLevelId() const {
    return _currLoadedLevelId;
  }

  void removeObstacle(const Point &nodePos);

private:
  bool isIntersectingObstacle(const Point &nodePos) const final;

  int32_t loadLevelFromDisk(const std::string &levelsFolderPath,
                            const int32_t levelId);

  void loadNextLevel();
  void loadPreviousLevel();

  using LevelObstacles = std::vector<Obstacle>;

  GridContainerProxyInterface *_gridInterface;

  std::vector<Point> _predefinedObstacles;
  std::vector<Point> _addedObstacles;
  std::vector<LevelObstacles> _levelsObstacles;

  int32_t _maxLevels;
  int32_t _currLoadedLevelId;
};

#endif /* GAMEENTITIES_OBSTACLEHANDLER_H_ */
