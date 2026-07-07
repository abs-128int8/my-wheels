#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <stdexcept>
#include "mctools/SocketClient.hpp"
#include "mctools/PacketReader.hpp"

namespace mywheels {
  SocketClient::SocketClient(std::string nodeName, std::string serviceName, int socketType) {
    if (socketType != SOCK_STREAM && socketType != SOCK_DGRAM) {
      throw std::invalid_argument("Invalid socket type. Use SOCK_STREAM or SOCK_DGRAM.");
    }
    _socketType = socketType;

    struct addrinfo hints;
    struct addrinfo *results;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = socketType;

    if (getaddrinfo(nodeName.c_str(), serviceName.c_str(), &hints, &results) != 0) {
      throw std::runtime_error("getaddrinfo() failed");
    }

    _socketFileDescriptor = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

    if (_socketFileDescriptor < 0) {
      freeaddrinfo(results);
      throw std::runtime_error("socket() failed");
    }

    if (connect(_socketFileDescriptor, results->ai_addr, results->ai_addrlen) < 0) {
      close(_socketFileDescriptor);
      freeaddrinfo(results);
      throw std::runtime_error("connect() failed");
    }

    freeaddrinfo(results);
  }

  SocketClient::~SocketClient() {
    if (_socketFileDescriptor >= 0) {
      close(_socketFileDescriptor);
    }
  }

  ssize_t SocketClient::sendData(const std::vector<std::uint8_t> &data) {
    ssize_t result = send(_socketFileDescriptor, data.data(), data.size(), 0);
    if (result < 0) {
      throw std::runtime_error("send() failed");
    }
    return result;
  }

  std::uint32_t SocketClient::readVarInt(const std::uint8_t *data, std::size_t length) {
    static constexpr std::uint32_t SEGMENT_BITS = 0x7F;
    static constexpr std::uint32_t CONTINUE_BIT = 0x80;

    std::uint32_t value = 0;
    std::size_t offset = 0;

    for (int position = 0; position < 32; position += 7) {
      if (offset >= length) {
        throw std::runtime_error("Unexpected end of buffer");
      }

      std::uint8_t currentByte = data[offset++];

      value |= (currentByte & SEGMENT_BITS) << position;

      if ((currentByte & CONTINUE_BIT) == 0) {
        return value;
      }
    }

    throw std::runtime_error("VarInt too large");
  }

  ssize_t SocketClient::receiveData(std::vector<std::uint8_t> &data) {
    data.clear();
    if (_socketType == SOCK_STREAM) {
      return receiveDataTcp(data);
    } else if (_socketType == SOCK_DGRAM) {
      return receiveDataUdp(data);
    } else {
      throw std::runtime_error("Unsupported socket type");
    }
  }

  ssize_t SocketClient::receiveDataTcp(std::vector<std::uint8_t> &data) {
    std::uint8_t buffer[4096];
    ssize_t totalReceived = 0;
    ssize_t result = recv(_socketFileDescriptor, buffer, sizeof(buffer), 0);
    std::uint32_t packetLength = readVarInt(buffer, result);
    data.insert(data.end(), buffer, buffer + result);
    totalReceived += result;

    while (totalReceived < packetLength) {
      result = recv(_socketFileDescriptor, buffer, sizeof(buffer), 0);
      if (result <= 0) {
        throw std::runtime_error("recv() failed or connection closed");
      }
      data.insert(data.end(), buffer, buffer + result);
      totalReceived += result;
    }

    return totalReceived;
  }

  ssize_t SocketClient::receiveDataUdp(std::vector<std::uint8_t> &data) {
    std::uint8_t buffer[4096];
    ssize_t result = recv(_socketFileDescriptor, buffer, sizeof(buffer), 0);
    if (result < 0) {
      throw std::runtime_error("recv() failed");
    }
    data.insert(data.end(), buffer, buffer + result);
    return result;
  }
} // namespace mywheels