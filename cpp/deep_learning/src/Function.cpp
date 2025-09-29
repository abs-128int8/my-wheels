#include "deep_learning/Function.hpp"
#include <numeric>

namespace mywheels {
  float sigmoid(float x) {
    return 1 / (1 + exp(-x));
  }

  Vecf softmax(Vecf v) {
    float max = *std::max_element(v.begin(), v.end());
    float sum = std::accumulate(v.begin(), v.end(), 0.0f, [max](float acc, float x) {
      return acc + exp(x - max);
    });
    return std::move(v)
             .apply([max](float x) {
               return x - max;
             })
             .apply(exp<float>)
           / sum;
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