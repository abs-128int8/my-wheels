#pragma once

namespace mywheels {
  float sqrt(float x);
  double sqrt(double x);

  float pow(float x, float y);
  double pow(double x, double y);

  float abs(float x);
  double abs(double x);

  float exp(float x);
  double exp(double x);

  template<typename T>
  T sigmoid(T x) {
    return 1 / (1 + exp(-x));
  }

  template<typename T>
  T step(T x) {
    return (x > 0) ? T(1) : T(0);
  }

  template<typename T>
  T lamp(T x) {
    return (x > 0) ? x : T(0);
  }

  template<typename T>
  T (*relu)(T) = lamp;
} // namespace mywheels
