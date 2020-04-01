#ifndef SDL_CONTAINERS_TEXTCONTAINER_H_
#define SDL_CONTAINERS_TEXTCONTAINER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers

//Forward declarations
class Color;
struct SDL_Texture;
typedef struct _TTF_Font TTF_Font;

class TextContainer {
public:
  TextContainer();
  ~TextContainer() = default;

  inline SDL_Texture* getTextTexture(const int32_t textId) const {
    return _textures[textId];
  }

  void createText(const char *text, const Color &color, const int32_t fontSize,
                  int32_t &outTextId, int32_t &outTextWidth,
                  int32_t &outTextHeight);

  void reloadText(const char *text, const Color &color, const int32_t fontSize,
                  const int32_t textId, int32_t &outTextWidth,
                  int32_t &outTextHeight);

  void unloadText(const int32_t textId);

protected:
  int32_t init();

  void deinit();

private:
  void occupyFreeSlotIndex(int32_t &occupiedTextId);
  void freeSlotIndex(const int32_t textId);

  int32_t loadFonts();

  //the global fonts we'll be using
  TTF_Font *_fontSmall;
  TTF_Font *_fontBig;

  //the textures we'll be drawing
  std::vector<SDL_Texture*> _textures;
};

#endif /* SDL_CONTAINERS_TEXTCONTAINER_H_ */
