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
    : _isCreated(false), _isVisible(true) {

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

void Widget::reset() {
  _drawParams.reset();
  _isCreated = false;
  _isVisible = true;
}

