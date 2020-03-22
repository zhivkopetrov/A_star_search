//Corresponding header
#include "ManagerHandler.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "DrawMgr.h"
#include "RsrcMgr.h"
#include "TimerMgr.h"
#include "utils/Log.h"

int32_t ManagerHandler::init(const ManagerHandlerCfg &cfg) {
  int32_t err = EXIT_SUCCESS;

  //gDrawMgr should be initialized first, because it contains the renderer
  //Other managers may want to load graphical resources
  gDrawMgr = new DrawMgr(cfg.displayMode, cfg.monitorWidth, cfg.monitorHeight);
  if (!gDrawMgr) {
    LOGERR("Error! Bad alloc for DrawMgr class -> Terminating...");

    err = EXIT_FAILURE;
  }

  gRsrcMgr = new RsrcMgr(cfg.monitorWidth, cfg.monitorHeight);
  if (!gRsrcMgr) {
    LOGERR("Error! Bad alloc for RsrcMgr class -> Terminating...");

    err = EXIT_FAILURE;
  }

  if (EXIT_SUCCESS == err) {
    gTimerMgr = new TimerMgr;
    if (!gTimerMgr) {
      LOGERR("Error! Bad alloc for TimerMgr class -> Terminating...");

      err = EXIT_FAILURE;
    }
  }

  if (EXIT_SUCCESS == err) {
    //put global managers into container so they can be easily iterated
    //and used polymorphically
    _managers[Managers::DRAW_MGR_IDX] = gDrawMgr;
    _managers[Managers::RSRC_MGR_IDX] = gRsrcMgr;
    _managers[Managers::TIMER_MGR_IDX] = gTimerMgr;
  }

  if (EXIT_SUCCESS == err) {
    for (int32_t i = 0; i < Managers::TOTAL_MGRS_COUNT; ++i) {
      if (EXIT_SUCCESS != _managers[i]->init()) {
        LOGERR("Error in %s init() -> Terminating...", _managers[i]->getName());
        err = EXIT_FAILURE;
        break;
      }
    }
  }

  return err;
}

void ManagerHandler::deinit() {
  /** Following the logic that DrawMgr should be initialized first ->
   * it should be deinitialized last
   * / a.k.a. last one to shut the door :) /
   * */
  for (int32_t i = Managers::TOTAL_MGRS_COUNT - 1; i >= 0; --i) {
    if (_managers[i]) //Sanity check
    {
      _managers[i]->deinit();

      delete _managers[i];
      _managers[i] = nullptr;

      nullifyGlobalManager(i);
    }
  }
}

void ManagerHandler::process() {
  for (int32_t i = 0; i < Managers::TOTAL_MGRS_COUNT; ++i) {
    _managers[i]->process();
  }
}

void ManagerHandler::nullifyGlobalManager(const int32_t managerId) {
  /** Explicitly set the singleton pointer to nullptr, because someone might
   * still try to use them -> if so, the sanity checks should catch them
   * */
  switch (managerId) {
  case Managers::DRAW_MGR_IDX:
    gDrawMgr = nullptr;
    break;

  case Managers::RSRC_MGR_IDX:
    gRsrcMgr = nullptr;
    break;

  case Managers::TIMER_MGR_IDX:
    gTimerMgr = nullptr;
    break;

  default:
    LOGERR("Unknown managerId: %d provided", managerId);
    break;
  }
}

