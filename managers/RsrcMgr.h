#ifndef MANAGERS_RSRCMGR_H_
#define MANAGERS_RSRCMGR_H_

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "MgrBase.h"

#include "sdl/TextureContainer.h"

//Forward declarations
struct SDL_Texture;

class RsrcMgr: public MgrBase {
public:
  RsrcMgr(const int32_t monitorWidth, const int32_t monitorHeight);
  virtual ~RsrcMgr() = default;

  //forbid the default constructor
  RsrcMgr() = delete;

  //forbid the copy and move constructors
  RsrcMgr(const RsrcMgr &other) = delete;
  RsrcMgr(RsrcMgr &&other) = delete;

  //forbid the copy and move assignment operators
  RsrcMgr& operator=(const RsrcMgr &other) = delete;
  RsrcMgr& operator=(RsrcMgr &&other) = delete;

  //================= START MgrBase related functions ====================

  /** @brief used to initialize the current manager.
   *         NOTE: this is the first function that will be called.
   *
   *  @return int32_t - error code
   * */
  virtual int32_t init() override;

  /** @brief used to deinitialize the current manager.
   * */
  virtual void deinit() override;

  /** @brief used to process the current manager (poll him on every
   *         engine cycle so the managers can do any internal updates, if
   *                                                     such are needed).
   * */
  virtual void process() override;

  /** @brief returns the name of the current manager
   *
   *  @return const char * - current manager name
   * */
  virtual const char* getName() override;

  //================== END MgrBase related functions =====================

  inline void setText(const char *text, const int32_t fontSize,
                      const uint8_t textureId, int32_t *outTextWidth,
                      int32_t *outTextHeight) {
    _textureContainer.setText(text, fontSize, textureId, outTextWidth,
        outTextHeight);
  }

  inline SDL_Texture* getTexture(const uint8_t textureId) const {
    return _textureContainer.getTexture(textureId);
  }

  inline SDL_Rect getTextureFrameRect(const uint8_t textureId,
                                      const uint8_t frameId) const {
    return _textureContainer.getTextureFrameRect(textureId, frameId);
  }

private:
  //container holding all the graphical textures
  TextureContainer _textureContainer;

  const int32_t _MONITOR_WIDTH;
  const int32_t _MONITOR_HEIGHT;
};

extern RsrcMgr *gRsrcMgr;

#endif /* MANAGERS_RSRCMGR_H_ */
