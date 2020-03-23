#ifndef GAMEENTITIES_PROXIES_PATHANIMATORPROXYINTERFACE_HPP_
#define GAMEENTITIES_PROXIES_PATHANIMATORPROXYINTERFACE_HPP_

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers

//Forward declarations

class PathAnimatorProxyInterface {
public:
  PathAnimatorProxyInterface() = default;
  virtual ~PathAnimatorProxyInterface() = default;

  virtual void onScaleAnimFinished() = 0;
};

#endif /* GAMEENTITIES_PROXIES_PATHANIMATORPROXYINTERFACE_HPP_ */
