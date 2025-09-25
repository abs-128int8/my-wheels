#pragma once

#include "math/Vector.hpp"

namespace mywheels {
  class SimplePerceptron {
  private:
    Vecf w;
    float b;

  public:
    SimplePerceptron(Vecf w, float b);
    float operator()(Vecf x);
    static float OR(Vecf x);
    static float AND(Vecf x);
    static float NAND(Vecf x);
    static float XOR(Vecf x);
  };
} // namespace mywheels