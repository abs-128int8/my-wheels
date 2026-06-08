#include "deeplearning/Function.hpp"
#include <numeric>

namespace mywheels {
  float sigmoid(float x) {
    if (x >= 0.0f) {
      return 1.0f / (1.0f + exp(-x));
    } else {
      return exp(x) / (1.0f + exp(x));
    }
  }

  Vecf softmax(Vecf v) {
    float max = v.max();
    v = std::move(v)
          .apply([max](float x) {
            return x - max;
          })
          .apply(exp<float>);
    return v / v.sum();
  }

  Matf softmax(Matf m) {
    Vecf ret = softmax(Vecf(std::move(m)));
    return Matf(ret);
  }

  float step(float x) {
    return (x > 0.0f) ? 1.0f : 0.0f;
  }

  float relu(float x) {
    return (x > 0.0f) ? x : 0.0f;
  }

  float identity(float x) {
    return x;
  }
} // namespace mywheels