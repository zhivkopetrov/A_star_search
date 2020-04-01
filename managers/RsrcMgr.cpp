//Corresponding header
#include "RsrcMgr.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "utils/Log.h"

RsrcMgr *gRsrcMgr = nullptr;

RsrcMgr::RsrcMgr(const int32_t monitorWidth, const int32_t monitorHeight)
    : _MONITOR_WIDTH(monitorWidth), _MONITOR_HEIGHT(monitorHeight) {
}

int32_t RsrcMgr::init() {
  int32_t err = EXIT_SUCCESS;
  if (EXIT_SUCCESS != TextureContainer::init(_MONITOR_WIDTH, _MONITOR_HEIGHT)) {
    LOGERR("Error in TextureContainer::init()");

    err = EXIT_FAILURE;
  }

  return err;
}

void RsrcMgr::deinit() {
  TextureContainer::deinit();
}

void RsrcMgr::process() {

}

const char* RsrcMgr::getName() {
  return "RsrcMgr";
}


