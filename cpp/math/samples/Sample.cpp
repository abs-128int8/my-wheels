#include "math/Matrix.hpp"

using namespace mywheels;

int main() {
  double sigma2_1 = (23 + std::sqrt(493)) / 2;
  double sigma2_2 = (23 - std::sqrt(493)) / 2;

  double v_11 = 11;
  double v_12 = -(13 - sigma2_1);
  double v_21 = 11;
  double v_22 = -(13 - sigma2_2);
  double norm_1 = std::sqrt(v_11 * v_11 + v_12 * v_12);
  double norm_2 = std::sqrt(v_21 * v_21 + v_22 * v_22);

  Matd V(2);
  V(0, 0) = v_11 / norm_1;
  V(1, 0) = v_12 / norm_1;
  V(0, 1) = v_21 / norm_2;
  V(1, 1) = v_22 / norm_2;

  std::cout << V << std::endl;

  Matd A({3, 3, 2, 1}, 2);

  double sigma_1 = std::sqrt(sigma2_1);
  double sigma_2 = std::sqrt(sigma2_2);

  Matd u_1 = A * V.col(0) / sigma_1;
  Matd u_2 = A * V.col(1) / sigma_2;

  Matd U(2);
  U(0, 0) = u_1(0, 0);
  U(1, 0) = u_1(1, 0);
  U(0, 1) = u_2(0, 0);
  U(1, 1) = u_2(1, 0);

  std::cout << U << std::endl;

  Matd Q = U * t(V);

  std::cout << Q << std::endl;

  return 0;
}