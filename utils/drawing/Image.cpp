//Corresponding header
#include "Image.h"

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "managers/RsrcMgr.h"
#include "utils/Log.h"

Image::Image()
    : _isDestroyed(false) {
  _drawParams.widgetType = WidgetType::IMAGE;
}

Image::~Image() {
  //attempt to destroy Image only if it's was first created and not destroyed
  if (true == _isCreated && false == _isDestroyed) {
    Image::destroy();
  }
}

void Image::create(const int32_t textureId) {
  if (_isCreated) {
    LOGERR("Error, Text with textureId: %d is already created", textureId);
    return;
  }

  const Frames &frames = gRsrcMgr->getImageFrames(textureId);
  _isCreated = true;
  _isDestroyed = false;
  _drawParams.rsrcId = textureId;
  _drawParams.frameId = 0;
  _drawParams.frameRect = frames[_drawParams.frameId];
  _drawParams.pos.x = 0;
  _drawParams.pos.y = 0;
  _drawParams.width = frames[_drawParams.frameId].w;
  _drawParams.height = frames[_drawParams.frameId].h;
}

void Image::destroy() {
  if (_isDestroyed) {
    LOGERR("Warning, trying to destroy already destroyed Image with rsrcId: %d",
        _drawParams.rsrcId);
    return;
  }
  _isDestroyed = true;
  Widget::reset();
}

void Image::setFrame(const int32_t frameId) {
  const Frames &frames = gRsrcMgr->getImageFrames(_drawParams.rsrcId);
  if (frameId >= static_cast<int32_t>(frames.size())) {
    LOGERR("Error, frameId: %d does not exist for Image with rsrcId: %d",
        frameId, _drawParams.rsrcId);
  }

  _drawParams.frameId = frameId;
  _drawParams.frameRect = frames[frameId];
}

