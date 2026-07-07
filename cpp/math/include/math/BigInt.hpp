#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <vector>

namespace mywheels {
  class BigInt {
  private:
    static constexpr int32_t BASE = 1000000000;           // 10^9
    static constexpr int64_t BASE2 = 1000000000000000000; // 10^18

    int m_sign;
    std::vector<uint32_t> _digits;

  public:
    BigInt() = default;
    template<std::integral T>
    BigInt(T n) {
      if (n == 0) {
        m_sign = 0;
        return;
      }

      m_sign = (n < 0) ? -1 : 1;

      using U = std::make_unsigned_t<T>;
      U unsigned_n = (n < 0) ? U(-(n + 1)) + 1 : U(n);

      while (unsigned_n > 0) {
        _digits.push_back(static_cast<uint32_t>(unsigned_n % BASE));
        unsigned_n /= BASE;
      }
    }

    BigInt(std::string_view s);
    std::string toString() const;

    void setZero();

    static BigInt zero();
  };
} // namespace mywheels