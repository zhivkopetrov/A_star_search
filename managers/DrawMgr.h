#ifndef MANAGERS_DRAWMGRBASE_H_
#define MANAGERS_DRAWMGRBASE_H_

//C system headers

//C++ system headers
#include <cstddef>

//Other libraries headers

//Own components headers
#include "MgrBase.h"

//Forward declarations
class SDLContainers;
class Renderer;
class MonitorWindow;
class Color;
struct DrawParams;
enum class BlendMode : uint8_t;

class DrawMgr: public MgrBase {
public:
  /** @brief
   *
   *  @param const int32_t   - monitor display mode
   *                                     (full screen, windowed, etc ...)
   *  @param const int32_t   - the width of the program monitor
   *  @param const int32_t   - the height of the program monitor
   * */
  explicit DrawMgr(const int32_t displayMode,
                   const int32_t monitorWidth,
                   const int32_t monitorHeight);

  //forbid the default constructor
  DrawMgr() = delete;

  //forbid the copy and move constructors
  DrawMgr(const DrawMgr &other) = delete;
  DrawMgr(DrawMgr &&other) = delete;

  //forbid the copy and move assignment operators
  DrawMgr& operator=(const DrawMgr &other) = delete;
  DrawMgr& operator=(DrawMgr &&other) = delete;

  //================= START MgrBase related functions ====================

  /** @brief used to initialize the current manager.
   *         NOTE: this is the first function that will be called.
   *
   *  @return int32_t - error code
   * */
  int32_t init() override;

  /** @brief used to deinitialize the current manager.
   * */
  void deinit() override;

  /** @brief used to process the current manager (poll him on every
   *         engine cycle so the managers can do any internal updates, if
   *                                                     such are needed).
   * */
  void process() override;

  /** @brief returns the name of the current manager
   *
   *  @return const char * - current manager name
   * */
  const char* getName() override;

  //================== END MgrBase related functions =====================

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

  /** @brief transfer draw specific data from Widgets to renderer
   *
   *  @param const DrawParams * - draw specific data for a single Widget
   * */
  void addDrawCmd(const DrawParams *drawParams);

  /** @brief transfer draw specific data from Textures to renderer
   *
   *  @param const DrawParams *[] - draw specific data for a Texture array
   *  @param const size_t         - size of the array
   * */
  void addDrawCmdArr(const DrawParams drawParamsArr[], const size_t size);

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

  /** @brief clear current renderer target (back buffer) with the selected color
   *
   * @param const Color & - target clear color
   * */
  void clearCurrentRendererTarget(const Color &clearColor);

  /** @brief transfer draw specific data from Textures to custom back buffer
   *         renderer target
   *
   *  @param const DrawParams *[] - draw specific data for a Texture array
   *  @param const size_t         - size of the array
   * */
  void updateCurrRendererTarget(const DrawParams drawParamsArr[],
                                const size_t size);

  /** @brief used to change the underlying texture blendmode for target widget
   *
   * @param const DrawParams & - target widget
   * @param const BlendMode    - target blendmode
   * */
  void setWidgetBlendMode(const DrawParams &widgetInfo,
                          const BlendMode blendMode);

  /** @brief used to change the target texture opacity (transparency)
   *
   * @param const DrawParams & - target widget
   * @param const uint8_t      - target opacity
   * */
  void setWidgetOpacity(const DrawParams &widgetInfo,
                        const uint8_t opacity);

  /** @brief used to limit the frame rate to a specific value.
   *         In order not to over burden the CPU, when the desired FPS
   *         is reached, the thread that executes the drawing is put
   *                                                             to sleep
   *
   *  @param const uint32_t - max frame cap
   * */
  inline void setMaxFrameRate(const uint32_t maxFrames) {
    _maxFrames = maxFrames;
  }

  /** @brief used to acquire the _maxFrames rate, which was set
   *
   *  @return uint32_t - max frames
   * */
  inline uint32_t getMaxFrameRate() const {
    return _maxFrames;
  }

  inline int32_t getMonitorWidth() const {
    return _MONITOR_WIDTH;
  }

  inline int32_t getMonitorHeight() const {
    return _MONITOR_HEIGHT;
  }

private:
  //Hide renderer implementation under user defined renderer class.
  //On later stages renderer internal implementation could be switched
  //to OPEN_GL one
  Renderer *_renderer;

  //The window we'll be rendering to
  MonitorWindow *_window;

  const int32_t _MONITOR_WIDTH;
  const int32_t _MONITOR_HEIGHT;

  //Holds the current window display mode (fullscreen or not)
  int32_t _displayMode;

  //Hold maximum frame rate cap
  uint32_t _maxFrames;
};

extern DrawMgr *gDrawMgr;

#endif /* MANAGERS_DRAWMGRBASE_H_ */

