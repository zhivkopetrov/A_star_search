#ifndef MANAGERS_RSRCMGR_H_
#define MANAGERS_RSRCMGR_H_

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "MgrBase.h"

#include "sdl/containers/TextureContainer.h"

//Forward declarations

class RsrcMgr: public MgrBase,
               public TextureContainer {
public:
  RsrcMgr(const int32_t monitorWidth, const int32_t monitorHeight);

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

private:
  const int32_t _MONITOR_WIDTH;
  const int32_t _MONITOR_HEIGHT;
};

extern RsrcMgr *gRsrcMgr;

#endif /* MANAGERS_RSRCMGR_H_ */
