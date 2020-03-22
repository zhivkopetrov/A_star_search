#ifndef GAMEENTITIES_ANIMATORS_GENERALTEXTANIMATOR_H_
#define GAMEENTITIES_ANIMATORS_GENERALTEXTANIMATOR_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "utils/drawing/Text.h"

//Forward declarations

class GeneralTextAnimator {
public:
  GeneralTextAnimator() = default;
  ~GeneralTextAnimator() = default;

  int32_t init();

  void draw();

  inline void showNoPathText() {
    _noPathText.show();
  }

  inline void hideNoPathText() {
    _noPathText.hide();
  }

private:
  Text _noPathText;
};

#endif /* GAMEENTITIES_ANIMATORS_GENERALTEXTANIMATOR_H_ */
