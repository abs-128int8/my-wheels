#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>
#include "mcje/SocketClient.hpp"

namespace mywheels {
  SocketClient::SocketClient(const char *nodeName, const char *serviceName, int socketType) {
    struct addrinfo hints;
    struct addrinfo *results;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = socketType;

    if (getaddrinfo(nodeName, serviceName, &hints, &results) != 0) {
      throw std::runtime_error("getaddrinfo() failed");
    }

    m_socketFileDescriptor = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    if (m_socketFileDescriptor < 0) {
      freeaddrinfo(results);
      throw std::runtime_error("socket() failed");
    }

    if (connect(m_socketFileDescriptor, results->ai_addr, results->ai_addrlen) < 0) {
      close(m_socketFileDescriptor);
      freeaddrinfo(results);
      throw std::runtime_error("connect() failed");
    }

    freeaddrinfo(results);
  }

  SocketClient::~SocketClient() {
    if (m_socketFileDescriptor >= 0) {
      close(m_socketFileDescriptor);
    }
  }

  ssize_t SocketClient::sendData(const std::vector<std::uint8_t> &data) {
    ssize_t result = send(m_socketFileDescriptor, data.data(), data.size(), 0);
    if (result < 0) {
      throw std::runtime_error("send() failed");
    }
    return result;
  }

  ssize_t SocketClient::receiveData(std::vector<std::uint8_t> &data) {
    std::uint8_t buffer[4096];
    ssize_t result = recv(m_socketFileDescriptor, buffer, sizeof(buffer), 0);

    if (result < 0) {
      throw std::runtime_error("recv() failed");
    }

    if (result == 0) {
      throw std::runtime_error("connection closed");
    }

    data.insert(data.end(), buffer, buffer + result);
    return result;
  }
} // namespace mywheels