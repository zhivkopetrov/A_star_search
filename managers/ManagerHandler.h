#ifndef MANAGERS_MANAGERHANDLER_H_
#define MANAGERS_MANAGERHANDLER_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations
class MgrBase;

namespace Managers {
enum Indexes {
  DRAW_MGR_IDX, RSRC_MGR_IDX, TIMER_MGR_IDX,

  TOTAL_MGRS_COUNT
};
}

struct ManagerHandlerCfg {
  explicit ManagerHandlerCfg(const int32_t inputDisplayMode,
                             const int32_t inputMonitorWidth,
                             const int32_t inputMonitorHeight)
      : displayMode(inputDisplayMode), monitorWidth(inputMonitorWidth),
        monitorHeight(inputMonitorHeight) {
  }
  ManagerHandlerCfg() = delete;

  const int32_t displayMode;
  const int32_t monitorWidth;
  const int32_t monitorHeight;
};

class ManagerHandler {
public:
  ManagerHandler() = default;
  ~ManagerHandler() = default;

  int32_t init(const ManagerHandlerCfg &cfg);

  /** @brief used to deinitialize all engine managers.
   * */
  void deinit();

  /** @brief used to process tall the engine managers (poll them on every
   *         engine cycle so the managers can do any internal updates, if
   *                                                     such are needed).
   * */
  void process();

  //================== END engine interface functions ====================

private:
  /** @brief used to reset the global manager pointer to nullptr at deinit
   *         so sanity checks could catch possible failures on system
   *         deinit
   *
   *  const int32_t - selected manager id
   * */
  void nullifyGlobalManager(const int32_t managerId);

  /** A container for all managers. We use polymorphical approach
   * so we can iterate it more easily
   * */
  MgrBase *_managers[Managers::TOTAL_MGRS_COUNT];
};

#endif /* MANAGERS_MANAGERHANDLER_H_ */

