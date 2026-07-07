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
    int _packetType = SOCK_STREAM;

  public:
    PacketReader(const std::vector<std::uint8_t> &buffer) : _buffer(buffer), _readPosition(0) {
    }

    PacketReader(const std::vector<std::uint8_t> &buffer, int packetType) :
      _buffer(buffer), _readPosition(0), _packetType(packetType) {
    }

    std::uint8_t readByte();
    std::vector<std::uint8_t> readBytes(std::size_t length);
    std::int32_t readVarInt();
    std::string readString();
    std::uint8_t readUint8();
    std::int8_t readInt8();
    std::uint16_t readUint16();
    std::int16_t readInt16();
    std::uint32_t readUint32();
    std::int32_t readInt32();
    std::uint64_t readUint64();
    std::int64_t readInt64();
    std::int16_t readInt16LittleEndian();
  };
} // namespace mywheels