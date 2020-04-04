//Corresponding header
#include "FBO.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "managers/DrawMgr.h"
#include "managers/RsrcMgr.h"

#include "utils/Log.h"

//default constructor
FBO::FBO() :
      _clearColor(Colors::BLACK), _itemsOffsetX(0), _itemsOffsetY(0),
      _isLocked(true), _isDestroyed(false) {
      _drawParams.widgetType = WidgetType::FBO;
}

FBO::~FBO() {
  //attempt to destroy the FBO only
  //if it was first created and not destroyed
  if (true == _isCreated && false == _isDestroyed) {
    destroy();
  }
}

void FBO::create(const int32_t startPosX, const int32_t startPosY,
                 const int32_t FBOWidth,  const int32_t FBOHeight) {
  if (_isCreated) {
    LOGERR("Warning, trying to create a FBO that was already created!");
  } else {
    //populate Widget's DrawParams
    _isCreated = true;
    _isDestroyed = false;

    _drawParams.pos.x = startPosX;
    _drawParams.pos.y = startPosY;
    _drawParams.width = FBOWidth;
    _drawParams.height = FBOHeight;
    _drawParams.frameRect.x = 0;
    _drawParams.frameRect.y = 0;
    _drawParams.frameRect.w = FBOWidth;
    _drawParams.frameRect.h = FBOHeight;

    gRsrcMgr->createFBO(FBOWidth, FBOHeight, _drawParams.FBOId);
  }
}

void FBO::destroy() {
  if (_isDestroyed) {
    LOGERR("Warning, trying to destroy a FBO " "that was already destroyed!");
    return;
  }

  if (!_isCreated) {
    LOGERR("Warning, trying to destroy a not-created sprite buffer");
    return;
  }

  //sanity check, because manager could already been destroyed
  if (nullptr != gRsrcMgr) {
    gRsrcMgr->destroyFBO(_drawParams.FBOId);
  }

  Widget::reset();
  resetInternals();

  _isDestroyed = true;
}

void FBO::draw() {
  if (!_isCreated) {
    LOGERR("Error, FBO was not created!");
  } else if (_isVisible) {
    gDrawMgr->addDrawCmd(&_drawParams);
  }
}

void FBO::unlock() {
  if (!_isCreated) {
    LOGERR("Error, FBO::unlock() failed, because FBO is not yet created. "
        "Consider using ::create() method first");
    return;
  }

  if (!_isLocked) {
    LOGERR("Error, trying to unlock a FBO " "that is already unlocked");
    return;
  }

  _isLocked = false;

  if (EXIT_SUCCESS != gDrawMgr->unlockRenderer()) {
    LOGERR(
        "Error, FBO can not be unlocked, because some other entity is "
        "currently in possession of the main renderer lock. Usually this is "
        "another FBO. Be sure to lock your FBOs when you are done with "
        "your work on them.");
    return;
  }

  gDrawMgr->changeRendererTarget(_drawParams.FBOId);
}

void FBO::lock() {
  if (!_isCreated) {
    LOGERR("Error, FBO::lock() failed, because FBO is not yet created. "
        "Consider using ::create() method first");
    return;
  }

  if (_isLocked) {
    LOGERR("Error, trying to lock a FBO that is already locked");
    return;
  }

  _isLocked = true;
  if (EXIT_SUCCESS != gDrawMgr->lockRenderer()) {
    LOGERR("gDrawMgr->lockRenderer() failed");
    return;
  }

  gDrawMgr->resetRendererTarget();
}

void FBO::reset() {
  if (!_isCreated) {
    LOGERR("Error, FBO::reset() failed, because FBO is not yet created. "
        "Consider using ::create() method first");
    return;
  }

  if (_isLocked) {
    LOGERR(
        "Error, FBO::reset() failed, because FBO is still locked. "
        "Consider using the sequence ::unlock(), ::reset(), ::lock()");
    return;
  }

  _storedItems.clear();

  gDrawMgr->clearCurrentRendererTarget(_clearColor);
}

void FBO::addWidget(const Widget &widget) {
  if (!widget.isCreated()) {
    LOGERR("Widget is not created, therefore -> it could not be added to FBO");
    return;
  }
  _storedItems.emplace_back(widget.getDrawParams());
}

void FBO::update() {
  if (!_isCreated) {
    LOGERR("Error, FBO::update() failed, because FBO is not yet created. "
        "Consider using ::create() method first");
    return;
  }

  if (_isLocked) {
    LOGERR("Error, FBO::update() failed, because FBO is still locked. "
        "Consider using the sequence " "::unlock(), ::update(), ::lock()");
    return;
  }

  //transform relative sprite buffer coordinates to
  //relative ones for the monitor, on which the FBO is attached to
  transformToMonitorRelativeCoordinates();

  //add the actual command and the data for the stored DrawParams
  gDrawMgr->updateCurrRendererTarget(_storedItems.data(), _storedItems.size());
}

void FBO::setClearColor(const Color& color) {
  if (!_isAlphaModulationEnabled && Colors::FULL_TRANSPARENT == color) {
    LOGERR("Error, FULL_TRANSPARENT color requires alpha modulation "
        "to be set first");
    return;
  }

  _clearColor = color;
}

void FBO::transformToMonitorRelativeCoordinates() {
  const int32_t SPRITE_BUFFER_POS_X = _drawParams.pos.x - _itemsOffsetX;
  const int32_t SPRITE_BUFFER_POS_Y = _drawParams.pos.y - _itemsOffsetY;

  for (auto & param : _storedItems) {
    param.pos.x -= SPRITE_BUFFER_POS_X;
    param.pos.y -= SPRITE_BUFFER_POS_Y;
  }
}

void FBO::resetInternals() {
  _storedItems.clear();

  _itemsOffsetX = 0;
  _itemsOffsetY = 0;
  _isLocked = true;
  _isDestroyed = false;
}


