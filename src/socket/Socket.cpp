#include "Socket.h"

namespace ymwm::socket {
  Socket::Socket(std::string host, unsigned short port)
      : m_socket(host, std::to_string(port), LIBSOCKET_IPv4)
      , m_stream(m_socket.accept2()) {}

  std::string Socket::read() const noexcept {
    std::string buffer;
    *m_stream >> buffer;
    return buffer;
  }

  void Socket::write(std::string data) { *m_stream << data; }
} // namespace ymwm::socket
