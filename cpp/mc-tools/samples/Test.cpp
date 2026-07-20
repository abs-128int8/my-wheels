#include <iostream>
#include <sys/socket.h>
#include <iomanip>
#include "mctools/SocketClient.hpp"
#include "mctools/PacketWriter.hpp"
#include "mctools/PacketReader.hpp"
#include "mctools/UDPSocketClient.hpp"

using namespace mywheels;

void printHex(const std::vector<std::uint8_t> &data) {
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

int main() {
  std::string host = "raspberrypi.local";
  std::string port = "25565";

  // Handshake
  PacketWriter packetWriter(SOCK_DGRAM);
  packetWriter.writeInt16(0xFEFD); // Magic
  packetWriter.writeInt8(0x09);    // Type
  packetWriter.writeInt32(0);      // Session ID
  auto packet = packetWriter.getPacket(0);
  std::cout << "Handshake Packet: " << std::endl;
  printHex(packet);

  UDPSocketClient udpSocketClient(host, port);
  udpSocketClient.sendPacket(packet);
  auto udpResponse = udpSocketClient.receivePacket();
  printHex(udpResponse);

  return 0;
}