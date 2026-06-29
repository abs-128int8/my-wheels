#include <iostream>
#include <sys/socket.h>
#include <iomanip>
#include "mcje/SocketClient.hpp"
#include "mcje/PacketWriter.hpp"
#include "mcje/PacketReader.hpp"

using namespace mywheels;

void printHex(const std::vector<std::uint8_t> &data) {
  for (std::uint8_t byte : data) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
  }
  std::cout << std::dec << std::endl;
}

int main() {
  SocketClient socketClient("raspberrypi.local", "25565", SOCK_STREAM);
  PacketWriter packetWriter;
  packetWriter.writeVarInt(775);
  packetWriter.writeString("raspberrypi.local");
  packetWriter.writeUint16(25565);
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
  packetWriter.writeLong(0x000000000014dc4c);
  packet = packetWriter.getPacket(1);
  printHex(packet);
  socketClient.sendData(packet);

  // Ping Response
  response.clear();
  socketClient.receiveData(response);
  printHex(response);
  packetReader = PacketReader(response);
  packetLength = packetReader.readVarInt();
  packetId = packetReader.readVarInt();
  auto pingResponse = packetReader.readLong();
  std::cout << "Packet Length: " << packetLength << std::endl;
  std::cout << "Packet ID: " << packetId << std::endl;
  std::cout << "Ping Response: " << pingResponse << std::endl;

  return 0;
}