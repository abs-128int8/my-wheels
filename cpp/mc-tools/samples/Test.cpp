#include <iostream>
#include <sys/socket.h>
#include <iomanip>
#include "mctools/SocketClient.hpp"
#include "mctools/PacketWriter.hpp"
#include "mctools/PacketReader.hpp"
#include "mctools/UDPSocketClient.hpp"
#include "mctools/Utils.hpp"

using namespace mywheels;

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
  Utils::printHex(packet);

  UDPSocketClient udpSocketClient(host, port);
  udpSocketClient.sendPacket(packet);
  auto udpResponse = udpSocketClient.receivePacket();
  Utils::printHex(udpResponse);

  return 0;
}