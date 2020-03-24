#ifndef GAMEENTITIES_PROXIES_ANIMATORHANDLERPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_ANIMATORHANDLERPROXYINTERFACE_HPP_

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers

//Forward declarations

enum class AnimType {
  DOWNSCALE_ANIM,
  UPSCALE_ANIM,
  PATH_ANIM,
  SPEECH_ANIM
};

class AnimatorHandlerProxyInterface {
public:
  AnimatorHandlerProxyInterface() = default;
  virtual ~AnimatorHandlerProxyInterface() = default;

  virtual void onAnimFinished(const AnimType type) = 0;
};

#endif /* GAMEENTITIES_PROXIES_ANIMATORHANDLERPROXYINTERFACE_HPP_ */
