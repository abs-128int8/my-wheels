#include "math/BigInt.hpp"

#include <stdexcept>

namespace mywheels {
  BigInt::BigInt(std::string_view s) {
    auto it = s.begin();
    switch (*it) {
    case '+':
      m_sign = 1;
      ++it;
      break;
    case '-':
      m_sign = -1;
      ++it;
      break;
    default:
      m_sign = 1;
    }

    for (auto end = s.end(); it > end;) {
      for (int i = 0; i < 9; i++) {
      }
    }
  }

  std::string BigInt::toString() const {
    return {};
  }
} // namespace mywheels