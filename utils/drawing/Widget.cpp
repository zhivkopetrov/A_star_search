//Corresponding header
#include "Widget.h"

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "managers/DrawMgr.h"
#include "utils/Log.h"

//default constructor
Widget::Widget()
    : _isCreated(false), _isVisible(true), _isAlphaModulationEnabled(false) {

}

//move constructor
Widget::Widget(Widget &&movedOther) {
  //take ownership of resources
  _drawParams = movedOther._drawParams;
  _isCreated = movedOther._isCreated;
  _isVisible = movedOther._isVisible;

  //ownership of resource should be taken from moved instance
  movedOther.reset();
}

//move assignment operator implementation
Widget& Widget::operator=(Widget &&movedOther) {
  //check for self-assignment
  if (this != &movedOther) {
    //take ownership of resources
    _drawParams = movedOther._drawParams;
    _isCreated = movedOther._isCreated;
    _isVisible = movedOther._isVisible;

    //ownership of resource should be taken from moved instance
    movedOther.reset();
  }

  return *this;
}

void Widget::draw() {
  if (_isVisible) {
    gDrawMgr->addDrawCmd(&_drawParams);
  }
}

bool Widget::containsPoint(const Point &point) const {
  const Rectangle bounds { _drawParams.pos.x, _drawParams.pos.y,
      _drawParams.width, _drawParams.height };

  return bounds.isPointInRect(point);
}

void Widget::activateAlphaModulation() {
  if (!_isCreated) {
    LOGERR("Error, alpha modulation can not be set, because Widget is not yet "
        "created");
    return;
  }

  if (_isAlphaModulationEnabled) {
    LOGERR("Error, alpha modulation is already set for Widget with Id: %d",
        _drawParams.rsrcId);
    return;
  }

  _isAlphaModulationEnabled = true;
  gDrawMgr->setWidgetBlendMode(_drawParams, BlendMode::BLEND);
}

void Widget::deactivateAlphaModulation() {
  if (!_isAlphaModulationEnabled) {
    LOGERR("Error, alpha modulation was not enabled for Widget with Id: %d",
        _drawParams.rsrcId);
    return;
  }

  _isAlphaModulationEnabled = false;
  gDrawMgr->setWidgetBlendMode(_drawParams, BlendMode::NONE);
}

void Widget::setOpacity(const int32_t opacity) {
  if (!_isAlphaModulationEnabled) {
    LOGERR("Error, alpha modulation was not enabled for Widget with Id: %d",
        _drawParams.rsrcId);
    return;
  }

  if(ZERO_OPACITY > opacity || FULL_OPACITY < opacity) {
    LOGERR("Error, opacity can only be in the range 0-255 while %d is provided "
        "for widget with ID: %d", opacity, _drawParams.rsrcId);
    return;
  }

  _drawParams.opacity = opacity;

  //WidgetType::IMAGE gets their opacity set in the actual draw cycle
  if (WidgetType::IMAGE != _drawParams.widgetType) {
    gDrawMgr->setWidgetOpacity(_drawParams, static_cast<uint8_t>(opacity));
  }
}

void Widget::reset() {
  _drawParams.reset();
  _isCreated = false;
  _isVisible = true;
}


