#include "deeplearning/SimplePerceptron.hpp"

namespace mywheels {
  SimplePerceptron::SimplePerceptron(Vecf w, float b) : w(w), b(b) {};

  float SimplePerceptron::operator()(Vecf x) {
    float y = dot(w, x) + b;
    if (y <= 0) {
      return 0.0f;
    } else {
      return 1.0f;
    }
  }

  float SimplePerceptron::OR(Vecf x) {
    return SimplePerceptron({0.5f, 0.5f}, -0.2f)(x);
  }

  float SimplePerceptron::AND(Vecf x) {
    return SimplePerceptron({0.5f, 0.5f}, -0.7f)(x);
  }

  float SimplePerceptron::NAND(Vecf x) {
    return SimplePerceptron({-0.5f, -0.5f}, 0.7f)(x);
  }

  float SimplePerceptron::XOR(Vecf x) {
    float s1 = SimplePerceptron::NAND(x);
    float s2 = SimplePerceptron::OR(x);
    return SimplePerceptron::AND({s1, s2});
  }
} // namespace mywheels