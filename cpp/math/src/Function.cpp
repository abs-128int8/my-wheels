#include "math/Function.hpp"
#include <cmath>

namespace mywheels {
  float sqrt(float x) {
    return std::sqrt(x);
  }

  double sqrt(double x) {
    return std::sqrt(x);
  }

  float pow(float x, float y) {
    return std::pow(x, y);
  }

  double pow(double x, double y) {
    return std::pow(x, y);
  }

  float abs(float x) {
    return std::fabs(x);
  }

  double abs(double x) {
    return std::fabs(x);
  }

} // namespace mywheels