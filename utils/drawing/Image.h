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
  Image() = default;
  ~Image() = default;

  //move constructor needed for STL containers emplace_back/push_back
  Image(Image &&movedOther) = default;

  //move assignment operator implementation
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

  void setFrame(const int32_t frameId);

  /** @brief used to change the underlying textureId
   *
   *  @param const int32_t - new unique texture ID
   * */
  inline void setTextureId(const int32_t textureId) {
    _drawParams.rsrcId = textureId;
  }
};

#endif /* UTILS_DRAWING_IMAGE_H_ */

