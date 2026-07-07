#pragma once

#include <cstdint>
#include <string_view>
#include <vector>
#include <sys/socket.h>

namespace mywheels {
  class PacketWriter {
  private:
    static constexpr std::uint32_t SEGMENT_BITS = 0x7F;
    static constexpr std::uint32_t CONTINUE_BIT = 0x80;
    std::vector<std::uint8_t> _buffer;
    int _packetType = SOCK_STREAM;

    void write(std::vector<std::uint8_t> data);

  public:
    PacketWriter() = default;
    PacketWriter(int packetType) : _packetType(packetType) {};

    void clear();

    void writeFront(std::vector<std::uint8_t> data);
    static std::vector<std::uint8_t> varInt(std::int32_t value);

    void writeVarInt(std::int32_t value);
    void writeString(std::string_view str);
    void writeUint8(std::uint8_t value);
    void writeInt8(std::int8_t value);
    void writeUint16(std::uint16_t value);
    void writeInt16(std::int16_t value);
    void writeUint32(std::uint32_t value);
    void writeInt32(std::int32_t value);
    void writeUint64(std::uint64_t value);
    void writeInt64(std::int64_t value);

    // packetId is unused for UDP
    const std::vector<std::uint8_t> &getPacket(int packetId);
  };
} // namespace mywheels