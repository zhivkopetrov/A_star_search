#ifndef SDL_RENDERER_H_
#define SDL_RENDERER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "utils/drawing/DrawParams.h"

//Forward declarations
struct SDL_Window;
struct SDL_Renderer;
class TextureContainer;

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
   *  @param const DrawParams * - draw specific data for a single Texture
   * */
  void drawTexture(const DrawParams *drawParams);

  /** @brief transfer draw specific data from Textures to renderer
   *
   *  @param const DrawParams *[] - draw specific data for a Texture array
   *  @param const size_t         - size of the array
   * */
  void drawTextureArr(const DrawParams drawParamsArr[], const size_t size);

  /* @brief locks the renderer - so it is able to draw again to the default
   *        back buffer
   *
   *  @return int32_t - error code
   * */
  int32_t lockRenderer();

  /** @brief locks the renderer - so it is able to draw again on custom
   *         back buffer
   *
   *  @return int32_t - error code
   * */
  int32_t unlockRenderer();

  /** @brief change renderer target to the FBO back buffer texture
   *
   *  @param const int32_t - unique FBO id (back buffer texture)
   * */
  void changeRendererTarget(const int32_t FBOId);

  /** @brief resets the renderer target to the default back buffer
   * */
  void resetRendererTarget();

  /** @brief transfer draw specific data from Textures to custom back buffer
   *         renderer target
   *
   *  @param const DrawParams *[] - draw specific data for a Texture array
   *  @param const size_t         - size of the array
   * */
  void updateCurrRendererTarget(const DrawParams drawParamsArr[],
                                const size_t size);

  /** @brief actual rendering of stored widgets on the current back buffer
   *
   *  @param const DrawParams *[] - draw specific data for a Texture array
   *  @param const size_t         - size of the array
   * */
  void drawStoredWidgets(const DrawParams drawParamsArr[],
                         const size_t size);

private:
  //The window we'll be rendering to
  SDL_Window *_window;

  //The Hardware Accelerated Renderer
  SDL_Renderer *_sdlRenderer;

  //counter for Textures in each individual frame
  size_t _currWidgetCounter;

  std::vector<DrawParams> _widgets;

  bool _isRendererLocked;
};

#endif /* SDL_RENDERER_H_ */

