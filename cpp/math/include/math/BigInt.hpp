#pragma once

#include <string>
#include <string_view>
#include <cstdint>
#include <vector>

namespace mywheels {
  class BigInt {
  private:
    static constexpr int32_t BASE = 1000000000; // 10^9

    int m_sign;
    std::vector<uint32_t> m_digits;

  public:
    BigInt() = default;

    BigInt(int32_t n);
    BigInt(int64_t n);
    BigInt(__int128_t n);
    BigInt(std::string_view s);

    int32_t toInt32() const;
    int64_t toInt64() const;
    __int128_t toInt128() const;
    std::string toString() const;

    void setZero();

    static BigInt zero();
  };
} // namespace mywheels