//Corresponding header
#include "Image.h"

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "managers/RsrcMgr.h"
#include "utils/Log.h"

void Image::create(const uint8_t textureId) {
  if (_isCreated) {
    LOGERR("Error, Text with textureId: %hhu is already created", textureId);
    return;
  }

  _isCreated = true;
  _drawParams.rsrcId = textureId;
  _drawParams.frameId = 0;

  const SDL_Rect frameRect = gRsrcMgr->getTextureFrameRect(textureId,
      _drawParams.frameId);
  _drawParams.pos.x = frameRect.x;
  _drawParams.pos.y = frameRect.y;
  _drawParams.width = frameRect.w;
  _drawParams.height = frameRect.h;
}

