//Corresponding header
#include "Text.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "managers/RsrcMgr.h"
#include "utils/Log.h"

Text::Text() : _fontSize(0), _isDestroyed(false) {
  _drawParams.widgetType = WidgetType::TEXT;
}

Text::~Text() {
  //attempt to destroy text only if it's was first created and not destroyed
  if(true == _isCreated && false == _isDestroyed) {
    Text::destroy();
  }
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
  _isDestroyed = false;
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

void Text::destroy() {
  if(_isDestroyed)
  {
     LOGERR("Warning, trying to destroy already destroyed text with textId: %d",
            _drawParams.textId);
     return;
  }

  if(!_isCreated)
  {
      LOGERR("Warning, trying to destroy a not-created text with fontId: %d",
            _drawParams.rsrcId);
      return;
  }

  //sanity check, because manager could already been destroyed
  if(nullptr != gRsrcMgr) {
      //unload text from graphical text vector
      gRsrcMgr->unloadText(_drawParams.textId);
  }

  _isDestroyed = true;
  Widget::reset();
}


