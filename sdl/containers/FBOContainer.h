#ifndef SDL_CONTAINERS_FBOCONTAINER_H_
#define SDL_CONTAINERS_FBOCONTAINER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "utils/drawing/Rectangle.h"

//Forward declarations
struct SDL_Texture;

class FBOContainer {
public:
  FBOContainer() = default;
  ~FBOContainer() = default;

  inline SDL_Texture* getFBOTexture(const int32_t FBOId) const {
    return _textures[FBOId];
  }

  void createFBO(const int32_t FBOWidth, const int32_t FBOHeight,
                 int32_t &outFBOId);

  void destroyFBO(const int32_t FBOId);

protected:
  int32_t init();

  void deinit();

private:
  void occupyFreeSlotIndex(int32_t &occupiedTextId);

  //the textures we'll be drawing
  std::vector<SDL_Texture*> _textures;
};

#endif /* SDL_CONTAINERS_FBOCONTAINER_H_ */
