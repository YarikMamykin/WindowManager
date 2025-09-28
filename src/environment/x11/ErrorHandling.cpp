#include "Handlers.h"
#include "log/Logger.h"

#include <array>
#include <format>

namespace ymwm::environment {
  int handle_x_error(Display* display, XErrorEvent* error) {
    std::array<char, 256> errorText;

    // Get the error message
    XGetErrorText(
        display, error->error_code, errorText.data(), errorText.size());

    log::Logger::error(std::format(
        "X Error: {}\nMajor opcode: {}\nMinor opcode: {}\nResource ID: {}",
        errorText.data(),
        error->request_code,
        error->minor_code,
        error->resourceid));

    // Return 0 to prevent the default error handler from being called
    return 0;
  }

  // Custom IO error handler
  int handle_x_io_error(Display* display) {
    log::Logger::error("Fatal X11 IO Error: Connection to X server lost\n");
    exit(1);
  }
} // namespace ymwm::environment
