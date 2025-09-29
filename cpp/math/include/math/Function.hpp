#pragma once

#include <cmath>

namespace mywheels {
  template<typename T>
  auto abs(const T &x) {
    using std::abs;
    return abs(x);
  }

  template<typename T>
  auto sqrt(const T &x) {
    using std::sqrt;
    return sqrt(x);
  }

  template<typename T>
  auto pow(const T &x, const T &y) {
    using std::pow;
    return pow(x, y);
  }

  template<typename T>
  auto exp(const T &x) {
    using std::exp;
    return exp(x);
  }
} // namespace mywheels