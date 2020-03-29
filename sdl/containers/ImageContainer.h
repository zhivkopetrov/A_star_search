#ifndef SDL_CONTAINERS_IMAGECONTAINER_H_
#define SDL_CONTAINERS_IMAGECONTAINER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "utils/drawing/Rectangle.h"
#include "common/CommonDefines.h"

//Forward declarations
struct SDL_Texture;

using Frames = std::vector<Rectangle>;

class ImageContainer {
public:
  ImageContainer();
  ~ImageContainer() = default;

  inline const Frames& getImageFrames(const int32_t rsrcId) const {
    return _textureFrameRects[rsrcId];
  }

  inline SDL_Texture* getImageTexture(const int32_t textureId) const {
    return _textures[textureId];
  }

protected:
  int32_t init(const int32_t windowWidth, const int32_t windowHeight);

  void deinit();

private:
  void populateTextureFrameRects(const int32_t windowWidth,
                                 const int32_t windowHeight);

  int32_t loadTextures();

  int32_t loadSingleTexture(const char *filePath, const int32_t textureId);

  //the textures we'll be drawing
  SDL_Texture* _textures[Textures::COUNT];

  //individual texture source frame rectangles
  Frames _textureFrameRects[Textures::COUNT];
};

#endif /* SDL_CONTAINERS_IMAGECONTAINER_H_ */
