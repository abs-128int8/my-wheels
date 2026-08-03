#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdexcept>
#include "mctools/UDPSocketClient.hpp"
#include "mctools/Utils.hpp"

namespace mywheels {
  UDPSocketClient::UDPSocketClient(const std::string &host, const std::string &port) {
    _socketFileDescriptor = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (_socketFileDescriptor < 0) {
      throw std::runtime_error("socket() failed");
    }

    struct addrinfo hints{};
    struct addrinfo *results = nullptr;
    hints.ai_family = AF_INET;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &results) != 0) {
      close(_socketFileDescriptor);
      throw std::runtime_error("getaddrinfo() failed");
    }

    if (connect(_socketFileDescriptor, results->ai_addr, results->ai_addrlen) < 0) {
      close(_socketFileDescriptor);
      freeaddrinfo(results);
      throw std::runtime_error("connect() failed");
    }

    _remoteAddress = *reinterpret_cast<struct sockaddr_in *>(results->ai_addr);
    freeaddrinfo(results);

    socklen_t len = sizeof(_localAddress);
    if (getsockname(_socketFileDescriptor, reinterpret_cast<struct sockaddr *>(&_localAddress), &len) < 0) {
      close(_socketFileDescriptor);
      throw std::runtime_error("getsockname() failed");
    }
  }

  UDPSocketClient::~UDPSocketClient() {
    if (_socketFileDescriptor >= 0) {
      close(_socketFileDescriptor);
    }
  }

  ssize_t UDPSocketClient::sendPacket(const std::vector<std::uint8_t> &data) {
    // UDP Header
    struct udphdr udpHeader{};
    udpHeader.source = 0;
    udpHeader.dest = _remoteAddress.sin_port;
    udpHeader.len = htons(sizeof(struct udphdr) + data.size());
    udpHeader.check = 0;

    // Pseudo IP Header
    struct pseudoIpHeader {
      struct in_addr sourceAddress;
      struct in_addr destAddress;
      uint8_t padding;
      uint8_t protocol;
      uint16_t udpLength;
    } pseudoIpHeader{};
    pseudoIpHeader.sourceAddress = _localAddress.sin_addr;
    pseudoIpHeader.destAddress = _remoteAddress.sin_addr;
    pseudoIpHeader.padding = 0;
    pseudoIpHeader.protocol = IPPROTO_UDP;
    pseudoIpHeader.udpLength = udpHeader.len;

    std::vector<std::uint8_t> pseudoPacket(sizeof(struct pseudoIpHeader) + sizeof(struct udphdr) + data.size());
    memcpy(pseudoPacket.data(), &pseudoIpHeader, sizeof(struct pseudoIpHeader));
    memcpy(pseudoPacket.data() + sizeof(struct pseudoIpHeader), &udpHeader, sizeof(struct udphdr));
    memcpy(pseudoPacket.data() + sizeof(struct pseudoIpHeader) + sizeof(struct udphdr), data.data(), data.size());
    udpHeader.check = Utils::calculateChecksum(pseudoPacket);

    std::vector<std::uint8_t> packet(sizeof(struct udphdr) + data.size());
    memcpy(packet.data(), &udpHeader, sizeof(struct udphdr));
    memcpy(packet.data() + sizeof(struct udphdr), data.data(), data.size());
    return send(_socketFileDescriptor, packet.data(), packet.size(), 0);
  }

  std::vector<std::uint8_t> UDPSocketClient::receivePacket() {
    std::vector<std::uint8_t> buffer(65536);
    ssize_t bytesRead = 0;
    while (true) {
      bytesRead = recv(_socketFileDescriptor, buffer.data(), buffer.size(), 0);
      if (bytesRead < 0) {
        throw std::runtime_error("recv() failed");
      }

      auto *ipHeader = reinterpret_cast<struct iphdr *>(buffer.data());
      if (ipHeader->protocol != IPPROTO_UDP) {
        continue;
      }

      auto ipHeaderLength = ipHeader->ihl * 4;
      auto *udpHeader = reinterpret_cast<struct udphdr *>(buffer.data() + ipHeaderLength);
      if (udpHeader->source != _remoteAddress.sin_port) {
        continue;
      }
      break;
    }
    buffer.resize(bytesRead);
    return buffer;
  }
} // namespace mywheels