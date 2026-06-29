#include "mcje/PacketWriter.hpp"

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

  void PacketWriter::writeUint16(std::uint16_t value) {
    write(std::vector<std::uint8_t>{static_cast<std::uint8_t>(value >> 8), static_cast<std::uint8_t>(value & 0xFF)});
  }

  const std::vector<std::uint8_t> &PacketWriter::getPacket(int packetId) {
    writeFront(varInt(packetId));
    writeFront(varInt(_buffer.size()));
    return _buffer;
  }
} // namespace mywheels