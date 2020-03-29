//Corresponding header
#include "Image.h"

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "managers/RsrcMgr.h"
#include "utils/Log.h"

void Image::create(const int32_t textureId) {
  if (_isCreated) {
    LOGERR("Error, Text with textureId: %d is already created", textureId);
    return;
  }

  const Frames &frames = gRsrcMgr->getImageFrames(textureId);
  _isCreated = true;
  _drawParams.widgetType = WidgetType::IMAGE;
  _drawParams.rsrcId = textureId;
  _drawParams.frameId = 0;
  _drawParams.frameRect = frames[_drawParams.frameId];
  _drawParams.pos.x = 0;
  _drawParams.pos.y = 0;
  _drawParams.width = frames[_drawParams.frameId].w;
  _drawParams.height = frames[_drawParams.frameId].h;
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

