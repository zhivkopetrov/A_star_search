//Corresponding header
#include "RsrcMgr.h"

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "utils/Log.h"

RsrcMgr *gRsrcMgr = nullptr;

RsrcMgr::RsrcMgr(const int32_t monitorWidth, const int32_t monitorHeight)
    : _MONITOR_WIDTH(monitorWidth), _MONITOR_HEIGHT(monitorHeight) {
}

int32_t RsrcMgr::init() {
  int32_t err = EXIT_SUCCESS;
  if (EXIT_SUCCESS != _textureContainer.init(_MONITOR_WIDTH, _MONITOR_HEIGHT)) {
    LOGERR("Error in _textureContainer.init()");

    err = EXIT_FAILURE;
  }

  return err;
}

void RsrcMgr::deinit() {
  _textureContainer.deinit();
}

void RsrcMgr::process() {

}

const char* RsrcMgr::getName() {
  return "RsrcMgr";
}


