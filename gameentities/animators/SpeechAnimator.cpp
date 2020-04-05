//Corresponding header
#include "SpeechAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "gameentities/proxies/AnimatorHandlerProxyInterface.hpp"
#include "utils/LimitValues.hpp"
#include "utils/Log.h"

SpeechAnimator::SpeechAnimator()
    : winPhases { "Kaboom", "Bazinga", "Me >> Joker", "GG WP" },
      _animatorHandlerInterface(nullptr), _waitTimerId(INIT_INT32_VALUE),
      _winPhraseCounter(0), _winSpeechBubbleRsrcId(INIT_INT32_VALUE),
      _loseSpeechBubbleRsrcId(INIT_INT32_VALUE), _isActive(false) {

}

int32_t SpeechAnimator::init(
    AnimatorHandlerProxyInterface *animatorHandlerInterface,
    const Point &startBatmanPos, const int32_t batmanRsrcId,
    const int32_t winSpeechBubbleRsrcId, const int32_t loseSpeechBubbleRsrcId,
    const int32_t waitTimerId) {

  _animatorHandlerInterface = animatorHandlerInterface;
  _winSpeechBubbleRsrcId = winSpeechBubbleRsrcId;
  _loseSpeechBubbleRsrcId = loseSpeechBubbleRsrcId;
  _waitTimerId = waitTimerId;
  _batman.create(batmanRsrcId);
  _batman.setPosition(startBatmanPos);

  constexpr int32_t SPEECH_BUBBLE_OFFSET_X =
      BatmanDimensions::BIG_BATMAN_WIDTH + 50;
  constexpr int32_t SPEECH_BUBBLE_OFFSET_Y = -150;
  _speechBubble.create(winSpeechBubbleRsrcId);
  _speechBubble.setPosition(startBatmanPos.x + SPEECH_BUBBLE_OFFSET_X,
      startBatmanPos.y + SPEECH_BUBBLE_OFFSET_Y);

  //the X value will be dynamically set
  constexpr int32_t SPEECH_OFFSET_Y = 195;
  _speechText.create(Point(0, _speechBubble.getY() + SPEECH_OFFSET_Y), "-",
      Colors::BLACK, FontSize::SMALL);

  return EXIT_SUCCESS;
}

void SpeechAnimator::draw() {
  _batman.draw();
  _speechBubble.draw();
  _speechText.draw();
}

void SpeechAnimator::activateAnim(const SpeecAnimType type) {
  _isActive = true;

  if (SpeecAnimType::WIN == type) {
    _speechBubble.setTextureId(_winSpeechBubbleRsrcId);
    const int32_t IDX = _winPhraseCounter % WIN_PHRASES_COUNT;
    _speechText.setText(winPhases[IDX].c_str());
    ++_winPhraseCounter;
  } else {
    _speechBubble.setTextureId(_loseSpeechBubbleRsrcId);
    _speechText.setText("No solution");
  }
  _speechText.setX(
      _speechBubble.getX() + ( (_speechBubble.getWidth()
          - _speechText.getWidth())
                              / 2));

  startTimer(3000, _waitTimerId, TimerType::ONESHOT);
}

void SpeechAnimator::onTimeout(const int32_t timerId) {
  if (_waitTimerId == timerId) {
    _isActive = false;
    _animatorHandlerInterface->onAnimFinished(AnimType::SPEECH_ANIM);
  } else {
    LOGERR("Received unknown timerId: %d", timerId);
  }
}

