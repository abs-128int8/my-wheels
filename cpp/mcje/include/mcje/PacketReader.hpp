#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace mywheels {
  class PacketReader {
  private:
    static constexpr std::uint32_t SEGMENT_BITS = 0x7F;
    static constexpr std::uint32_t CONTINUE_BIT = 0x80;
    std::vector<std::uint8_t> _buffer;
    std::size_t _readPosition = 0;

  public:
    PacketReader(const std::vector<std::uint8_t> &buffer) : _buffer(buffer) {
    }
    std::uint8_t readByte();
    std::vector<std::uint8_t> readBytes(std::size_t length);
    std::int32_t readVarInt();
    std::string readString();
  };
} // namespace mywheels