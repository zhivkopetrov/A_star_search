//Corresponding header
#include "Text.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "managers/RsrcMgr.h"
#include "utils/Log.h"

Text::Text() : _fontSize(0) {

}

void Text::create(const uint8_t rsrcId, const Point &startPoint,
                  const char *startText, const int32_t fontSize) {
  if (_isCreated) {
    LOGERR("Error, Text with rsrcId: %hhu is already created", rsrcId);
    return;
  }

  _isCreated = true;
  _drawParams.rsrcId = rsrcId;
  _drawParams.pos = startPoint;
  _fontSize = fontSize;

  gRsrcMgr->setText(startText, fontSize, rsrcId, &_drawParams.width,
      &_drawParams.height);
}

void Text::setText(const char *text) {
  gRsrcMgr->setText(text, _fontSize, _drawParams.rsrcId, &_drawParams.width,
      &_drawParams.height);
}

