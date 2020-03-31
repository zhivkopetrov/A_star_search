#ifndef SDL_CONTAINERS_TEXTURECONTAINER_H_
#define SDL_CONTAINERS_TEXTURECONTAINER_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "ImageContainer.h"
#include "TextContainer.h"
#include "FBOContainer.h"

//Forward declarations

class TextureContainer : public ImageContainer,
                         public TextContainer,
                         public FBOContainer{
public:
  TextureContainer();
  ~TextureContainer() = default;

protected:
  int32_t init(const int32_t windowWidth, const int32_t windowHeight);

  void deinit();
};

#endif /* SDL_CONTAINERS_TEXTURECONTAINER_H_ */

