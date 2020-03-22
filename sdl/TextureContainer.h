#ifndef SDL_TEXTURECONTAINER_H_
#define SDL_TEXTURECONTAINER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers
#include <SDL2/SDL_rect.h>

//Own components headers

//Forward declarations
struct SDL_Texture;
struct SDL_Renderer;
typedef struct _TTF_Font TTF_Font;

class TextureContainer {
public:
  TextureContainer();

  virtual ~TextureContainer() = default;

  int32_t init(const int32_t windowWidth, const int32_t windowHeight);

  void deinit();

  void setText(const char *text, const int32_t fontSize,
               const uint8_t textureId, int32_t *outTextWidth,
               int32_t *outTextHeight);

  inline SDL_Texture* getTexture(const uint8_t textureId) const {
    return _textures[textureId];
  }

  inline SDL_Rect getTextureFrameRect(const uint8_t textureId) const {
    return _textureFrameRects[textureId];
  }

private:
  int32_t loadFonts();

  int32_t loadTextures(const int32_t windowWidth,
                       const int32_t windowHeight);

  int32_t loadSingleTexture(const char *filePath, const uint8_t textureId);

  void populateTextureFrameRects(const int32_t windowWidth,
                                 const int32_t windowHeight);

  //the textures we'll be drawing
  std::vector<SDL_Texture*> _textures;

  //individual texture source frame rectangles
  std::vector<SDL_Rect> _textureFrameRects;

  //the global fonts we'll be using
  TTF_Font *_fontSmall;
  TTF_Font *_fontBig;

  //the color we'll be using for the text
  SDL_Color _color;
};

#endif /* SDL_TEXTURECONTAINER_H_ */

