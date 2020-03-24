#ifndef GAMEENTITIES_ANIMATORS_SPEECHANIMATOR_H_
#define GAMEENTITIES_ANIMATORS_SPEECHANIMATOR_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <string>

//Other libraries headers

//Own components headers
#include "utils/drawing/Text.h"
#include "utils/drawing/Image.h"
#include "utils/time/TimerClient.h"

//Forward declarations
class AnimatorHandlerProxyInterface;

enum class SpeecAnimType {
  WIN,
  LOSE
};

class SpeechAnimator : public TimerClient {
public:
  SpeechAnimator();
  virtual ~SpeechAnimator() = default;

  int32_t init(AnimatorHandlerProxyInterface * animatorHandlerInterface,
               const Point &startBatmanPos, const uint8_t batmanRsrcId,
               const uint8_t winSpeechBubbleRsrcId,
               const uint8_t loseSpeechBubbleRsrcId,
               const int32_t waitTimerId);

  void draw();

  void activateAnim(const SpeecAnimType type);

  inline bool isActive() const {
    return _isActive;
  }

private:
  virtual void onTimeout(const int32_t timerId) override final;

  enum InternalDefines {
    WIN_PHRASES_COUNT = 3
  };

  Image _batman;
  Image _speechBubble;
  Text _speechText;

  const std::string winPhases[WIN_PHRASES_COUNT];

  AnimatorHandlerProxyInterface * _animatorHandlerInterface;

  int32_t _waitTimerId;
  int32_t _winPhraseCounter;

  uint8_t _winSpeechBubbleRsrcId;
  uint8_t _loseSpeechBubbleRsrcId;

  bool _isActive;
};

#endif /* GAMEENTITIES_ANIMATORS_SPEECHANIMATOR_H_ */
