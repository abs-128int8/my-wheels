#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <netinet/in.h>

namespace mywheels {
  class UDPSocketClient {
  private:
    int _socketFileDescriptor;
    struct sockaddr_in _address;

  public:
    UDPSocketClient(const std::string &host, const std::string &port);
    ~UDPSocketClient();

    ssize_t sendPacket(const std::vector<std::uint8_t> &data);
    std::vector<std::uint8_t> receivePacket();
  };
} // namespace mywheels