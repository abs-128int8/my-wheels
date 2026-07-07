#include "mctools/PacketWriter.hpp"

namespace mywheels {
  void PacketWriter::clear() {
    _buffer.clear();
  }

  void PacketWriter::write(std::vector<std::uint8_t> data) {
    _buffer.insert(_buffer.end(), data.begin(), data.end());
  }

  void PacketWriter::writeFront(std::vector<std::uint8_t> data) {
    _buffer.insert(_buffer.begin(), data.begin(), data.end());
  }

  std::vector<std::uint8_t> PacketWriter::varInt(std::int32_t value) {
    std::vector<std::uint8_t> buffer;
    while (true) {
      if ((value & ~SEGMENT_BITS) == 0) {
        buffer.push_back(static_cast<std::uint8_t>(value));
        return buffer;
      }
      buffer.push_back(static_cast<std::uint8_t>((value & SEGMENT_BITS) | CONTINUE_BIT));
      value >>= 7;
    }
  }

  void PacketWriter::writeVarInt(std::int32_t value) {
    write(varInt(value));
  }

  void PacketWriter::writeString(std::string_view str) {
    writeVarInt(str.size());
    write(std::vector<std::uint8_t>(str.begin(), str.end()));
  }

  void PacketWriter::writeUint8(std::uint8_t value) {
    write(std::vector<std::uint8_t>{value});
  }

  void PacketWriter::writeInt8(std::int8_t value) {
    write(std::vector<std::uint8_t>{static_cast<std::uint8_t>(value)});
  }

  void PacketWriter::writeUint16(std::uint16_t value) {
    write(std::vector<std::uint8_t>{static_cast<std::uint8_t>(value >> 8), static_cast<std::uint8_t>(value & 0xFF)});
  }

  void PacketWriter::writeInt16(std::int16_t value) {
    writeUint16(static_cast<std::uint16_t>(value));
  }

  void PacketWriter::writeUint32(std::uint32_t value) {
    write(std::vector<std::uint8_t>{
      static_cast<std::uint8_t>(value >> 24), static_cast<std::uint8_t>((value >> 16) & 0xFF),
      static_cast<std::uint8_t>((value >> 8) & 0xFF), static_cast<std::uint8_t>(value & 0xFF)});
  }

  void PacketWriter::writeInt32(std::int32_t value) {
    writeUint32(static_cast<std::uint32_t>(value));
  }

  void PacketWriter::writeUint64(std::uint64_t value) {
    write(std::vector<std::uint8_t>{
      static_cast<std::uint8_t>((value >> 56) & 0xFF), static_cast<std::uint8_t>((value >> 48) & 0xFF),
      static_cast<std::uint8_t>((value >> 40) & 0xFF), static_cast<std::uint8_t>((value >> 32) & 0xFF),
      static_cast<std::uint8_t>((value >> 24) & 0xFF), static_cast<std::uint8_t>((value >> 16) & 0xFF),
      static_cast<std::uint8_t>((value >> 8) & 0xFF), static_cast<std::uint8_t>(value & 0xFF)});
  }

  void PacketWriter::writeInt64(std::int64_t value) {
    writeUint64(static_cast<std::uint64_t>(value));
  }

  const std::vector<std::uint8_t> &PacketWriter::getPacket(int packetId) {
    if (_packetType == SOCK_STREAM) {
      writeFront(varInt(packetId));
      writeFront(varInt(_buffer.size()));
    }
    return _buffer;
  }
} // namespace mywheels