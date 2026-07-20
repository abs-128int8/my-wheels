#include <iostream>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdexcept>
#include "mctools/UDPSocketClient.hpp"

namespace mywheels {
  UDPSocketClient::UDPSocketClient(const std::string &host, const std::string &port) {
    _socketFileDescriptor = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (_socketFileDescriptor < 0) {
      throw std::runtime_error("socket() failed");
    }

    struct addrinfo hints{};
    struct addrinfo *results;
    hints.ai_family = AF_INET;
    if (int ret = getaddrinfo(host.c_str(), port.c_str(), &hints, &results) != 0) {
      close(_socketFileDescriptor);
      throw std::runtime_error("getaddrinfo() failed");
    }

    if (connect(_socketFileDescriptor, results->ai_addr, results->ai_addrlen) < 0) {
      close(_socketFileDescriptor);
      freeaddrinfo(results);
      throw std::runtime_error("connect() failed");
    }

    _address = *reinterpret_cast<struct sockaddr_in *>(results->ai_addr);
    freeaddrinfo(results);
  }

  UDPSocketClient::~UDPSocketClient() {
    if (_socketFileDescriptor >= 0) {
      close(_socketFileDescriptor);
    }
  }

  ssize_t UDPSocketClient::sendPacket(const std::vector<std::uint8_t> &data) {
    struct udphdr udpHeader;
    udpHeader.source = 0;
    udpHeader.dest = _address.sin_port;
    udpHeader.len = htons(sizeof(struct udphdr) + data.size());
    udpHeader.check = 0;
    std::vector<std::uint8_t> packet(sizeof(struct udphdr) + data.size());
    memcpy(packet.data(), &udpHeader, sizeof(struct udphdr));
    memcpy(packet.data() + sizeof(struct udphdr), data.data(), data.size());
    return send(_socketFileDescriptor, packet.data(), packet.size(), 0);
  }

  std::vector<std::uint8_t> UDPSocketClient::receivePacket() {
    std::vector<std::uint8_t> buffer(65536);
    ssize_t bytesRead = recv(_socketFileDescriptor, buffer.data(), buffer.size(), 0);
    if (bytesRead < 0) {
      throw std::runtime_error("recv() failed");
    }
    buffer.resize(bytesRead);
    return buffer;
  }
} // namespace mywheels