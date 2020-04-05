//Corresponding header
#include "ObstacleHandler.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <algorithm>
#include <fstream>

//Other libraries headers

//Own components headers
#include "utils/Log.h"

ObstacleHandler::ObstacleHandler()
    : _gridInterface(nullptr), _maxLevels(0), _currLoadedLevelId(0) {

}

int32_t ObstacleHandler::init(GridContainerProxyInterface *gridInterface,
                              const std::string &levelsFolder,
                              const int32_t levelsCount) {
  _gridInterface = gridInterface;
  _maxLevels = levelsCount;

  _levelsObstacles.resize(levelsCount);
  for (int32_t i = 0; i < levelsCount; ++i) {
    if (EXIT_SUCCESS != loadLevelFromDisk(levelsFolder, i)) {
      LOGERR("Error, loadLevel() failed for level id: %d", i);
      return EXIT_FAILURE;
    }
  }

  _gridInterface->placePredefinedObstacles(_levelsObstacles[_currLoadedLevelId]);
  // load the actual obstacles
  for (const auto & elem : _levelsObstacles[_currLoadedLevelId]) {
    _predefinedObstacles.push_back(elem.pos);
  }

  return EXIT_SUCCESS;
}

void ObstacleHandler::changeLevel(const std::any &value) {
  try {
    const auto levelOffset = std::any_cast<int32_t>(value);
    if (1 == levelOffset) {
      loadNextLevel();
    } else if (-1 == levelOffset) {
      loadPreviousLevel();
    } else {
      LOGERR("Received unknown level offset: %d", levelOffset);
    }
  } catch (const std::bad_any_cast &e) {
    LOGERR("any_cast throwed: %s", e.what());
  }
}

void ObstacleHandler::removeObstacle(const Point &nodePos) {
  auto it = std::find(_addedObstacles.begin(), _addedObstacles.end(), nodePos);
  if (it != _addedObstacles.end()) {
    _addedObstacles.erase(it);
  }
}

bool ObstacleHandler::isIntersectingObstacle(const Point &nodePos) const {
  const bool isIntersectingPredefined = std::find(_predefinedObstacles.begin(),
                                            _predefinedObstacles.end(), nodePos)
                                        != _predefinedObstacles.end();
  const bool isIntersectingAdded = std::find(_addedObstacles.begin(),
                                       _addedObstacles.end(), nodePos)
                                   != _addedObstacles.end();

  return isIntersectingPredefined || isIntersectingAdded;
}

int32_t ObstacleHandler::loadLevelFromDisk(const std::string &levelsFolderPath,
                                           const int32_t levelId) {
  std::string levelName = levelsFolderPath;
  levelName.append("level").append(std::to_string(levelId + 1)).append(".txt");
  std::ifstream ifstream(levelName.c_str(), std::ios::in | std::ios::binary);
  if (!ifstream) {
    LOGERR("Error, could not open file: %s, Reason: %s", levelName.c_str(),
        strerror(errno));
    return EXIT_FAILURE;
  }

  constexpr int32_t unknownTextureFrameId = 0;

  int32_t rows = 0;
  int32_t cols = 0;
  int32_t textureFrame = 0;
  ifstream >> cols >> rows;
  for (int32_t row = 0; row < rows; ++row) {
    for (int32_t col = 0; col < cols; ++col) {
      ifstream >> textureFrame;
      if (unknownTextureFrameId != textureFrame) {
        _levelsObstacles[levelId].emplace_back(col, row, textureFrame);
      }
    }
  }

  return EXIT_SUCCESS;
}

void ObstacleHandler::loadNextLevel() {
  const int32_t prevLevel = _currLoadedLevelId;
  ++_currLoadedLevelId;
  if (_currLoadedLevelId == _maxLevels) {
    _currLoadedLevelId = 0;
  }
  _gridInterface->removePredefinedObstacles(_levelsObstacles[prevLevel]);
  _gridInterface->placePredefinedObstacles(
      _levelsObstacles[_currLoadedLevelId]);

  //clear the actual obstacles and load the new ones
  _predefinedObstacles.clear();
  for (const auto & elem : _levelsObstacles[_currLoadedLevelId]) {
    _predefinedObstacles.push_back(elem.pos);
  }
}

void ObstacleHandler::loadPreviousLevel() {
  const int32_t prevLevel = _currLoadedLevelId;
  --_currLoadedLevelId;
  if (0 > _currLoadedLevelId) {
    _currLoadedLevelId = _maxLevels - 1;
  }
  _gridInterface->removePredefinedObstacles(_levelsObstacles[prevLevel]);
  _gridInterface->placePredefinedObstacles(
      _levelsObstacles[_currLoadedLevelId]);

  //clear the actual obstacles and load the new ones
  _predefinedObstacles.clear();
  for (const auto & elem : _levelsObstacles[_currLoadedLevelId]) {
    _predefinedObstacles.push_back(elem.pos);
  }
}

