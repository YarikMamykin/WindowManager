#include "Handlers.h"

#include <array>
#include <iostream>

namespace ymwm::environment {
  int handle_x_error(Display* display, XErrorEvent* error) {
    std::array<char, 256> errorText;

    // Get the error message
    XGetErrorText(
        display, error->error_code, errorText.data(), errorText.size());

    std::cerr << "X Error: " << errorText.data() << '\n'
              << "  Major opcode: " << error->request_code << '\n'
              << "  Minor opcode: " << error->minor_code << '\n'
              << "  Resource ID: " << error->resourceid << '\n';

    // Return 0 to prevent the default error handler from being called
    return 0;
  }

  // Custom IO error handler
  int handle_x_io_error(Display* display) {
    std::cerr << "Fatal X11 IO Error: Connection to X server lost\n";
    exit(1);
  }
} // namespace ymwm::environment
