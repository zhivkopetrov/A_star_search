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
               const Point &startBatmanPos, const int32_t batmanRsrcId,
               const int32_t winSpeechBubbleRsrcId,
               const int32_t loseSpeechBubbleRsrcId,
               const int32_t waitTimerId);

  void draw();

  void activateAnim(const SpeecAnimType type);

  inline bool isActive() const {
    return _isActive;
  }

private:
  virtual void onTimeout(const int32_t timerId) override final;

  enum InternalDefines {
    WIN_PHRASES_COUNT = 4
  };

  Image _batman;
  Image _speechBubble;
  Text _speechText;

  const std::string winPhases[WIN_PHRASES_COUNT];

  AnimatorHandlerProxyInterface * _animatorHandlerInterface;

  int32_t _waitTimerId;
  int32_t _winPhraseCounter;

  int32_t _winSpeechBubbleRsrcId;
  int32_t _loseSpeechBubbleRsrcId;

  bool _isActive;
};

#endif /* GAMEENTITIES_ANIMATORS_SPEECHANIMATOR_H_ */
