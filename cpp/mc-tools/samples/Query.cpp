#include <iostream>
#include <sys/socket.h>
#include <iomanip>
#include "mctools/SocketClient.hpp"
#include "mctools/PacketWriter.hpp"
#include "mctools/PacketReader.hpp"

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
  SocketClient socketClient(host, port, SOCK_DGRAM);
  PacketWriter packetWriter(SOCK_DGRAM);
  packetWriter.writeInt16(0xFEFD); // Magic
  packetWriter.writeInt8(0x09);    // Type
  packetWriter.writeInt32(0);      // Session ID
  auto packet = packetWriter.getPacket(0);
  std::cout << "Handshake Packet: " << std::endl;
  printHex(packet);
  socketClient.sendData(packet);

  std::vector<std::uint8_t> response;
  socketClient.receiveData(response);
  std::cout << "Handshake Response: " << std::endl;
  printHex(response);
  PacketReader packetReader(response, SOCK_DGRAM);
  auto type1 = packetReader.readInt8();
  auto sessionID1 = packetReader.readInt32();
  auto challengeToken1 = packetReader.readString();
  std::cout << "Type: " << static_cast<int>(type1) << std::endl;
  std::cout << "Session ID: " << sessionID1 << std::endl;
  std::cout << "Challenge Token: " << challengeToken1 << std::endl;

  // Basic Stat
  packetWriter.clear();
  packetWriter.writeInt16(0xFEFD);                     // Magic
  packetWriter.writeInt8(0);                           // Type
  packetWriter.writeInt32(0);                          // Session ID
  packetWriter.writeInt32(std::stoi(challengeToken1)); // Challenge Token
  packet = packetWriter.getPacket(0);
  std::cout << "Basic Stat Packet: " << std::endl;
  printHex(packet);
  socketClient.sendData(packet);

  socketClient.receiveData(response);
  std::cout << "Basic Stat Response: " << std::endl;
  printHex(response);
  packetReader = PacketReader(response, SOCK_DGRAM);
  auto type2 = packetReader.readInt8();
  auto sessionID2 = packetReader.readInt32();
  auto MOTD2 = packetReader.readString();
  auto gametype2 = packetReader.readString();
  auto map2 = packetReader.readString();
  auto numplayers2 = packetReader.readString();
  auto maxplayers2 = packetReader.readString();
  auto hostport2 = packetReader.readInt16LittleEndian();
  auto hostip2 = packetReader.readString();
  std::cout << "Type: " << static_cast<int>(type2) << std::endl;
  std::cout << "Session ID: " << sessionID2 << std::endl;
  std::cout << "MOTD: " << MOTD2 << std::endl;
  std::cout << "Gametype: " << gametype2 << std::endl;
  std::cout << "Map: " << map2 << std::endl;
  std::cout << "NumPlayers: " << numplayers2 << std::endl;
  std::cout << "MaxPlayers: " << maxplayers2 << std::endl;
  std::cout << "HostPort: " << hostport2 << std::endl;
  std::cout << "HostIP: " << hostip2 << std::endl;

  // Full Stat
  packetWriter.clear();
  packetWriter.writeInt16(0xFEFD);                     // Magic
  packetWriter.writeInt8(0);                           // Type
  packetWriter.writeInt32(0);                          // Session ID
  packetWriter.writeInt32(std::stoi(challengeToken1)); // Challenge Token
  packetWriter.writeInt32(0);                          // Padding
  packet = packetWriter.getPacket(0);
  std::cout << "Full Stat Packet: " << std::endl;
  printHex(packet);
  socketClient.sendData(packet);

  socketClient.receiveData(response);
  std::cout << "Full Stat Response: " << std::endl;
  printHex(response);
  packetReader = PacketReader(response, SOCK_DGRAM);
  auto type3 = packetReader.readInt8();
  auto sessionID3 = packetReader.readInt32();
  auto padding13 = packetReader.readBytes(11);
  // K, V section
  std::vector<std::pair<std::string, std::string>> kvsection3;
  while (true) {
    auto key = packetReader.readString();
    if (key == "") {
      break;
    }
    auto value = packetReader.readString();
    kvsection3.emplace_back(key, value);
  }
  auto padding23 = packetReader.readBytes(10);
  // Player section
  std::vector<std::string> playersection3;
  while (true) {
    auto playername = packetReader.readString();
    if (playername == "") {
      break;
    }
    playersection3.emplace_back(playername);
  }
  std::cout << "Type: " << static_cast<int>(type3) << std::endl;
  std::cout << "Session ID: " << sessionID3 << std::endl;
  std::cout << "Padding: " << std::endl;
  printHex(padding13);
  std::cout << "K, V Section:" << std::endl;
  for (const auto &[key, value] : kvsection3) {
    std::cout << "  " << key << ": " << value << std::endl;
  }
  std::cout << "Padding: " << std::endl;
  printHex(padding23);
  std::cout << "Player Section:" << std::endl;
  for (const auto &playername : playersection3) {
    std::cout << "  " << playername << std::endl;
  }

  return 0;
}