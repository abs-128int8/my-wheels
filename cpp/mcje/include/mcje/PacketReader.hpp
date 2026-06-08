#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace mywheels {
  class PacketReader {
  private:
    static constexpr std::uint32_t SEGMENT_BITS = 0x7F;
    static constexpr std::uint32_t CONTINUE_BIT = 0x80;
    std::vector<std::uint8_t> m_buffer;
    std::size_t m_readPosition = 0;

  public:
    std::int32_t readVarInt();
    std::string readString();
  };
} // namespace mywheels