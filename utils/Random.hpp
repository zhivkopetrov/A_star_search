#ifndef UTILS_RANDOM_HPP_
#define UTILS_RANDOM_HPP_

//C system headers

//C++ system headers
#include <cstdint>
#include <ctime>
#include <random>

//Other libraries headers

//Own components headers

//Forward declarations

class Random {
public:
  static Random& getInstance() {
    static Random random;
    return random;
  }

  inline int32_t generate(const int32_t lowerLimit, const int32_t upperLimit) {
    std::uniform_int_distribution<> range(lowerLimit, upperLimit);

    return range(gen);
  }

private:
  Random() {
    gen.seed(time(nullptr));
  }

  std::mt19937 gen;
};

#endif /* UTILS_RANDOM_HPP_ */
