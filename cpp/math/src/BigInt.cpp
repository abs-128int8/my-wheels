#include "math/BigInt.hpp"

namespace mywheels {
  BigInt::BigInt(int32_t n) {
    if (n == 0) {
      m_sign = 0;
    } else if (n < 0) {
      m_sign = -1;
      n = -n;
    } else {
      m_sign = 1;
    }

    if (n < BASE) {
      m_digits.push_back(static_cast<uint32_t>(n));
    } else {
      m_digits.push_back(static_cast<uint32_t>(n % BASE));
      m_digits.push_back(static_cast<uint32_t>(n / BASE));
    }
  }
} // namespace mywheels