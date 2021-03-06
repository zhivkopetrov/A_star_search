#ifndef UTILS_DRAWING_IMAGE_H_
#define UTILS_DRAWING_IMAGE_H_

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers
#include "Widget.h"

//Forward declarations
class Sprite;

class Image: public Widget {
public:
  Image();
  ~Image();

  Image(Image &&movedOther) = default;
  Image& operator=(Image &&movedOther) = default;

  //forbid the copy constructor and copy assignment operator
  Image(const Image &other) = delete;
  Image& operator=(const Image &other) = delete;

  /** @brief used to create resource. This function must be called
   *         in order to operate will the resource. The Image itself only
   *         handles draw specific data, not the actual Surface/Texture
   *   This function does not return error code for performance reasons
   *
   *  @param const int32_t - unique resource ID
   * */
  void create(const int32_t textureId);

  void destroy();

  void setFrame(const int32_t frameId);

  /** @brief used to change the underlying textureId
   *
   *  @param const int32_t - new unique texture ID
   * */
  inline void setTextureId(const int32_t textureId) {
    _drawParams.rsrcId = textureId;
  }

private:
  /* used in order to check if resource was destroyed ->
   *                                              not to destroy it twice
   */
  bool _isDestroyed;
};

#endif /* UTILS_DRAWING_IMAGE_H_ */

