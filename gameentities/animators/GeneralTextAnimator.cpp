//Corresponding header
#include "GeneralTextAnimator.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"

int32_t GeneralTextAnimator::init() {
  constexpr auto text = "No possible path found";

  _noPathText.create(Textures::NO_AVAILABLE_PATH_TEXT,
      Point(410, 460), text, FontSize::BIG);
  _noPathText.hide();

  return EXIT_SUCCESS;
}

void GeneralTextAnimator::draw() {
  _noPathText.draw();
}
