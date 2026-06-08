#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace mywheels {
  class PacketWriter {
  private:
    static constexpr std::uint32_t SEGMENT_BITS = 0x7F;
    static constexpr std::uint32_t CONTINUE_BIT = 0x80;
    std::vector<std::uint8_t> m_buffer;

  public:
    void clear();

    void write(std::vector<std::uint8_t> data);
    void writeFront(std::vector<std::uint8_t> data);
    static std::vector<std::uint8_t> varInt(std::int32_t value);

    void writeVarInt(std::int32_t value);
    void writeString(std::string_view str);
    void writeUint16(std::uint16_t value);
    const std::vector<std::uint8_t> &getPacket(int packetId);
  };
} // namespace mywheels