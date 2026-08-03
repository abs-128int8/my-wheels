#pragma once

#include <cstdint>
#include <vector>

namespace mywheels {
  class Utils {
  public:
    static std::uint16_t calculateChecksum(const std::vector<std::uint8_t> &data);
    static void printHex(const std::vector<std::uint8_t> &data);
  };
} // namespace mywheels