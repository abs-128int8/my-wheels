#include <iostream>
#include <iomanip>
#include <arpa/inet.h>
#include "mctools/Utils.hpp"

namespace mywheels {
  std::uint16_t Utils::calculateChecksum(const std::vector<std::uint8_t> &data) {
    std::uint32_t sum = 0;
    std::uint32_t word = 0;

    for (std::size_t i = 0; i + 1 < data.size(); i += 2) {
      word = static_cast<std::uint32_t>(data[i]) << 8 | static_cast<std::uint32_t>(data[i + 1]);
      sum += word;
      if (sum > 0xFFFF) {
        sum = (sum & 0xFFFF) + 1;
      }
    }

    if (data.size() % 2 == 1) {
      word = static_cast<std::uint32_t>(data.back()) << 8;
      sum += word;
      if (sum > 0xFFFF) {
        sum = (sum & 0xFFFF) + 1;
      }
    }

    std::uint16_t checksum = ~static_cast<std::uint16_t>(sum & 0xFFFF);
    if (checksum == 0) {
      checksum = 0xFFFF;
    }
    checksum = htons(checksum);

    return checksum;
  }

  void Utils::printHex(const std::vector<std::uint8_t> &data) {
    std::size_t size = data.size();
    for (std::size_t offset = 0; offset < size; offset += 16) {
      std::cout << std::hex << std::setfill('0') << std::setw(8) << offset << ": ";

      for (std::size_t i = 0; i < 16; i++) {
        if (offset + i < size) {
          std::cout << std::setw(2) << static_cast<int>(data[offset + i]) << " ";
        } else {
          std::cout << "   ";
        }
      }

      std::cout << " ";

      for (std::size_t i = 0; i < 16; i++) {
        if (offset + i < size) {
          char c = static_cast<char>(data[offset + i]);
          if (std::isprint(static_cast<unsigned char>(c))) {
            std::cout << c;
          } else {
            std::cout << ".";
          }
          std::cout << " ";
        }
      }
      std::cout << std::endl;
    }
    std::cout << std::dec;
  }
} // namespace mywheels