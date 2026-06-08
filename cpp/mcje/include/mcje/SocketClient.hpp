#pragma once

#include <cstdint>
#include <vector>
#include <sys/types.h>

namespace mywheels {
  class SocketClient {
  private:
    int m_socketFileDescriptor = -1;

  public:
    SocketClient(const char *nodeName, const char *serviceName, int socketType);
    ~SocketClient();

    ssize_t sendData(const std::vector<std::uint8_t> &data);
    ssize_t receiveData(std::vector<std::uint8_t> &data);
  };
} // namespace mywheels