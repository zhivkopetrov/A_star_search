#ifndef SDL_RENDERER_H_
#define SDL_RENDERER_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/drawing/DrawParams.h"

//Forward declarations
struct SDL_Window;
struct SDL_Renderer;

class Renderer {
public:
  Renderer(SDL_Window *window);

  Renderer() = delete;

  //forbid the copy and move constructors
  Renderer(const Renderer &other) = delete;
  Renderer(Renderer &&other) = delete;

  //forbid the copy and move assignment operators
  Renderer& operator=(const Renderer &other) = delete;
  Renderer& operator=(Renderer &&other) = delete;

  ~Renderer() = default;

  /** @brief used to initialise actual renderer
   *
   *  @returns int32_t     - error code
   * */
  int32_t init();

  /** @brief used to destroy renderer and window
   * */
  void deinit();

  //=====================================================================
  /** @brief Every frame should start with this function call
   * */
  void clearScreen();

  /** All the drawing functions calls should be encapsulated
   *   between clearScreen() and finishFrame() functions
   * */

  /** @brief Every frame should end with this function call
   * */
  void finishFrame();
  //=====================================================================

  /** @brief transfer draw specific data from Textures to renderer
   *
   *  @param DrawParams * - draw specific data for a single Texture
   * */
  void drawTexture(DrawParams *drawParams);

  /** @brief transfer draw specific data from Textures to renderer
   *
   *  @param DrawParams ** - draw specific data for a Texture array
   *  @param const int32_t - size of the array
   * */
  void drawTextureArr(DrawParams drawParamsArr[], const int32_t size);

private:
  enum {
    MAX_WIDGET_COUNT = 1000
  };

  //The window we'll be rendering to
  SDL_Window *_window;

  //The Hardware Accelerated Renderer
  SDL_Renderer *_sdlRenderer;

  //counter for Textures in each individual frame
  uint32_t _currWidgetCounter;

  DrawParams _widgets[MAX_WIDGET_COUNT];
};

#endif /* SDL_RENDERER_H_ */

