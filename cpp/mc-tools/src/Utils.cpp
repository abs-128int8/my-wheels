#include "mctools/Utils.hpp"

namespace mywheels {
  std::uint16_t Utils::calculateChecksum(const std::vector<std::uint8_t> &data) {
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i < data.size(); i += 2) {
      std::uint16_t word = data[i];
      if (i + 1 < data.size()) {
        word = (word << 8) | data[i + 1];
      }
      sum += word;
    }

    while (sum >> 16) {
      sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return static_cast<std::uint16_t>(~sum);
  }
} // namespace mywheels