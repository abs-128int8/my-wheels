#include "mcje/PacketReader.hpp"
#include <stdexcept>
#include <iostream>

namespace mywheels {
  std::uint8_t PacketReader::readByte() {
    if (_readPosition >= _buffer.size()) {
      throw std::runtime_error("readByte() failed: read position exceeds buffer size");
    }
    return _buffer[_readPosition++];
  }

  std::vector<std::uint8_t> PacketReader::readBytes(std::size_t length) {
    if (_readPosition + length > _buffer.size()) {
      throw std::runtime_error("readBytes() failed: read position exceeds buffer size");
    }
    std::vector<std::uint8_t> result(_buffer.begin() + _readPosition, _buffer.begin() + _readPosition + length);
    _readPosition += length;
    return result;
  }

  std::int32_t PacketReader::readVarInt() {
    std::int32_t value = 0;
    for (int position = 0; position < 32; position += 7) {
      auto currentByte = readByte();
      value |= (std::int32_t)(currentByte & SEGMENT_BITS) << position;

      if ((currentByte & CONTINUE_BIT) == 0) {
        return value;
      }
    }

    throw std::runtime_error("readVarInt() failed: too big");
  }

  std::string PacketReader::readString() {
    auto length = readVarInt();
    auto bytes = readBytes(length);
    return std::string(bytes.begin(), bytes.end());
  }
} // namespace mywheels