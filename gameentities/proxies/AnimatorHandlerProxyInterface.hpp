#ifndef GAMEENTITIES_PROXIES_ANIMATORHANDLERPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_ANIMATORHANDLERPROXYINTERFACE_HPP_

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers

//Forward declarations

class AnimatorHandlerProxyInterface {
public:
  AnimatorHandlerProxyInterface() = default;
  virtual ~AnimatorHandlerProxyInterface() = default;

  virtual void onAnimFinished() = 0;
};

#endif /* GAMEENTITIES_PROXIES_ANIMATORHANDLERPROXYINTERFACE_HPP_ */
