#include <iostream>
#include <sys/socket.h>
#include <iomanip>
#include "mctools/SocketClient.hpp"
#include "mctools/PacketWriter.hpp"
#include "mctools/PacketReader.hpp"

using namespace mywheels;

void printHex(const std::vector<std::uint8_t> &data) {
  std::cout << std::hex << std::setfill('0');
  for (std::uint8_t byte : data) {
    std::cout << std::setw(2) << static_cast<int>(byte) << " ";
  }
  std::cout << std::dec << std::endl;
}

int main() {
  // std::string host = "raspberrypi.local";
  std::string host = "mc.hypixel.net";
  std::string port = "25565";

  SocketClient socketClient(host, port, SOCK_STREAM);
  PacketWriter packetWriter;
  packetWriter.writeVarInt(775);
  packetWriter.writeString(host);
  packetWriter.writeUint16(std::stoi(port));
  packetWriter.writeVarInt(1);
  auto packet = packetWriter.getPacket(0);
  printHex(packet);
  socketClient.sendData(packet);

  packetWriter.clear();

  // Status Request
  packet = packetWriter.getPacket(0);
  printHex(packet);
  socketClient.sendData(packet);

  // Status Response
  std::vector<std::uint8_t> response;
  socketClient.receiveData(response);
  printHex(response);
  PacketReader packetReader(response);
  auto packetLength = packetReader.readVarInt();
  auto packetId = packetReader.readVarInt();
  auto jsonResponse = packetReader.readString();
  std::cout << "Packet Length: " << packetLength << std::endl;
  std::cout << "Packet ID: " << packetId << std::endl;
  std::cout << "JSON Response: " << jsonResponse << std::endl;

  // Ping Request
  packetWriter.clear();
  packetWriter.writeInt64(0x000000000014dc4c);
  packet = packetWriter.getPacket(1);
  printHex(packet);
  socketClient.sendData(packet);

  // Ping Response
  socketClient.receiveData(response);
  printHex(response);
  packetReader = PacketReader(response);
  packetLength = packetReader.readVarInt();
  packetId = packetReader.readVarInt();
  auto pingResponse = packetReader.readInt64();
  std::cout << "Packet Length: " << packetLength << std::endl;
  std::cout << "Packet ID: " << packetId << std::endl;
  std::cout << "Ping Response: " << pingResponse << std::endl;

  return 0;
}