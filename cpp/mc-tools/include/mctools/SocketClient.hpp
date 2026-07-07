#pragma once

#include <cstdint>
#include <vector>
#include <sys/types.h>

namespace mywheels {
  class SocketClient {
  private:
    int _socketFileDescriptor = -1;
    int _socketType;
    std::uint32_t readVarInt(const std::uint8_t *data, std::size_t length);
    ssize_t receiveDataTcp(std::vector<std::uint8_t> &data);
    ssize_t receiveDataUdp(std::vector<std::uint8_t> &data);

  public:
    SocketClient(std::string nodeName, std::string serviceName, int socketType);
    ~SocketClient();

    ssize_t sendData(const std::vector<std::uint8_t> &data);
    ssize_t receiveData(std::vector<std::uint8_t> &data);
  };
} // namespace mywheels