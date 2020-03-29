//Corresponding header
#include "Text.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "managers/RsrcMgr.h"
#include "utils/Log.h"

Text::Text()
    : _fontSize(0) {

}

void Text::create(const Point &startPoint, const char *text,
                  const int32_t fontSize) {
  if (_isCreated) {
    LOGERR("Error, Text with textId: %d is already created",
        _drawParams.textId);
    return;
  }

  gRsrcMgr->createText(text, fontSize, _drawParams.textId, _drawParams.width,
      _drawParams.height);

  _isCreated = true;
  _drawParams.widgetType = WidgetType::TEXT;
  _drawParams.frameId = 0;
  _drawParams.pos = startPoint;
  _fontSize = fontSize;
  _drawParams.frameRect.x = 0;
  _drawParams.frameRect.y = 0;
  _drawParams.frameRect.w = _drawParams.width;
  _drawParams.frameRect.h = _drawParams.height;
}

void Text::setText(const char *text) {
  gRsrcMgr->reloadText(text, _fontSize, _drawParams.textId,
      _drawParams.width, _drawParams.height);

  _drawParams.frameRect.w = _drawParams.width;
  _drawParams.frameRect.h = _drawParams.height;
}

