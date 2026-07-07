#include <stdexcept>
#include <iostream>
#include <sys/socket.h>
#include "mctools/PacketReader.hpp"

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
    if (_packetType == SOCK_STREAM) {
      auto length = readVarInt();
      std::cout << _buffer.size() << std::endl;
      std::cout << length << std::endl;
      auto bytes = readBytes(length);
      return std::string(bytes.begin(), bytes.end());
    } else if (_packetType == SOCK_DGRAM) {
      std::string result;
      while (_readPosition < _buffer.size()) {
        auto currentByte = readByte();
        if (currentByte == 0) {
          break;
        }
        result += static_cast<char>(currentByte);
      }
      return result;
    } else {
      throw std::runtime_error("readString() failed: unknown socket type");
    }
  }

  std::uint8_t PacketReader::readUint8() {
    if (_readPosition >= _buffer.size()) {
      throw std::runtime_error("readUint8() failed: read position exceeds buffer size");
    }
    return readByte();
  }

  std::int8_t PacketReader::readInt8() {
    if (_readPosition >= _buffer.size()) {
      throw std::runtime_error("readInt8() failed: read position exceeds buffer size");
    }
    return static_cast<std::int8_t>(readUint8());
  }

  std::uint16_t PacketReader::readUint16() {
    if (_readPosition + 2 > _buffer.size()) {
      throw std::runtime_error("readUint16() failed: read position exceeds buffer size");
    }
    std::uint16_t value = 0;
    auto bytes = readBytes(2);
    for (int i = 0; i < 2; ++i) {
      value |= static_cast<std::uint16_t>(bytes[i]) << (8 * (1 - i));
    }
    return value;
  }

  std::int16_t PacketReader::readInt16() {
    if (_readPosition + 2 > _buffer.size()) {
      throw std::runtime_error("readInt16() failed: read position exceeds buffer size");
    }
    std::int16_t value = static_cast<std::int16_t>(readUint16());
    return value;
  }

  std::uint32_t PacketReader::readUint32() {
    if (_readPosition + 4 > _buffer.size()) {
      throw std::runtime_error("readUint32() failed: read position exceeds buffer size");
    }
    std::uint32_t value = 0;
    auto bytes = readBytes(4);
    for (int i = 0; i < 4; ++i) {
      value |= static_cast<std::uint32_t>(bytes[i]) << (8 * (3 - i));
    }
    return value;
  }

  std::int32_t PacketReader::readInt32() {
    if (_readPosition + 4 > _buffer.size()) {
      throw std::runtime_error("readInt32() failed: read position exceeds buffer size");
    }
    std::int32_t value = static_cast<std::int32_t>(readUint32());
    return value;
  }

  std::uint64_t PacketReader::readUint64() {
    if (_readPosition + 8 > _buffer.size()) {
      throw std::runtime_error("readUint64() failed: read position exceeds buffer size");
    }
    std::uint64_t value = 0;
    auto bytes = readBytes(8);
    for (int i = 0; i < 8; ++i) {
      value |= static_cast<std::uint64_t>(bytes[i]) << (8 * (7 - i));
    }
    return value;
  }

  std::int64_t PacketReader::readInt64() {
    if (_readPosition + 8 > _buffer.size()) {
      throw std::runtime_error("readInt64() failed: read position exceeds buffer size");
    }
    std::int64_t value = static_cast<std::int64_t>(readUint64());
    return value;
  }

  std::int16_t PacketReader::readInt16LittleEndian() {
    if (_readPosition + 2 > _buffer.size()) {
      throw std::runtime_error("readInt16LittleEndian() failed: read position exceeds buffer size");
    }
    std::int16_t value = 0;
    auto bytes = readBytes(2);
    value |= static_cast<std::int16_t>(bytes[0]);
    value |= static_cast<std::int16_t>(bytes[1]) << 8;
    return value;
  }
} // namespace mywheels