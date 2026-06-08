#pragma once

#include "math/Matrix.hpp"

namespace mywheels {
  float sigmoid(float x);
  Vecf softmax(Vecf v);
  Matf softmax(Matf v);
  float step(float x);
  float relu(float x);
  float identity(float x);
} // namespace mywheels