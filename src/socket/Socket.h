#include "libsocket/inetclientstream.hpp"

#include <libsocket/exception.hpp>
#include <libsocket/inetserverstream.hpp>
#include <libsocket/socket.hpp>
#include <memory>

namespace ymwm::socket {
  class Socket {

  public:
    Socket(std::string host, unsigned short port);

    std::string read() const noexcept;

    void write(std::string data);

  private:
    libsocket::inet_stream_server m_socket;
    std::unique_ptr<libsocket::inet_stream> m_stream;
  };
} // namespace ymwm::socket
