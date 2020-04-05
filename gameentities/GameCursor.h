#ifndef GAMEENTITIES_GAMECURSOR_H_
#define GAMEENTITIES_GAMECURSOR_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward Declarations

class GameCursor {
public:
  GameCursor() = default;
  ~GameCursor();

  int32_t init(const char *imagePath, const int32_t cursorClickX,
               const int32_t cursorClickY);

  void deinit();
};

#endif /* GAMEENTITIES_GAMECURSOR_H_ */
