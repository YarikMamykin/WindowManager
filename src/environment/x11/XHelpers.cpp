#include "Handlers.h"
#include "common/Color.h"
#include "environment/ID.h"
#include "log/Logger.h"

#include <array>
#include <format>
#include <memory>
#include <string>

namespace ymwm::environment {

  XColor xcolor_from_color(const common::Color& color) noexcept {
    static constinit char flags = DoRed | DoGreen | DoBlue;
    static constinit auto byte = sizeof(unsigned char) * 8;
    // Bit shift required for XColor specifically in case values are in
    // standart 0..255 range, otherwise colors are rendered as very dark gray
    // (close to black).
    return XColor{ .red = color.red > 0x00ff
                              ? color.red
                              : static_cast<unsigned short>(color.red << byte),
                   .green = color.green > 0x00ff ? color.green
                                                 : static_cast<unsigned short>(
                                                       color.green << byte),
                   .blue = color.blue > 0x00ff ? color.blue
                                               : static_cast<unsigned short>(
                                                     color.blue << byte),
                   .flags = flags };
  }

  std::u8string get_window_name(ymwm::environment::Handlers& handlers,
                                Window w) noexcept {
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char* data_raw = nullptr;
    // RAII guard on data_raw allocated memory
    std::unique_ptr<unsigned char> data_safe_wrapper{ data_raw };
    std::u8string wname;

    // Try getting name through UTF8 atom
    int status = XGetWindowProperty(handlers.display,
                                    w,
                                    handlers.atoms.front(),
                                    0,
                                    (~0L),
                                    False,
                                    handlers.atoms.back(),
                                    &actual_type,
                                    &actual_format,
                                    &nitems,
                                    &bytes_after,
                                    &data_raw);
    if (Success == status and data_raw) {
      wname = std::u8string(reinterpret_cast<char8_t*>(data_raw));
    } else {
      // Try getting name through XA_WM_NAME atom
      status = XGetWindowProperty(handlers.display,
                                  w,
                                  XA_WM_NAME,
                                  0,
                                  (~0L),
                                  False,
                                  XA_STRING,
                                  &actual_type,
                                  &actual_format,
                                  &nitems,
                                  &bytes_after,
                                  &data_raw);

      if (status == Success and data_raw) {
        wname = std::u8string(reinterpret_cast<char8_t*>(data_raw));
      }
    }

    return wname;
  }

  bool register_colors(const std::array<ymwm::common::Color, 3ul>& colors,
                       Handlers& handlers) {
    for (const auto& c : colors) {
      handlers.colors.insert({ c, xcolor_from_color(c) });
      if (not XAllocColor(
              handlers.display, handlers.colormap, &handlers.colors.at(c))) {
        log::Logger::error(std::format("Failed to allocate color: {} {} {}\n",
                                       handlers.colors.at(c).red,
                                       handlers.colors.at(c).green,
                                       handlers.colors.at(c).blue));
        return false;
      }
    }

    return true;
  }

  void x_send_expose_event(Handlers& handlers, ID window_id) noexcept {
    XEvent expose_event;
    expose_event.type = Expose;
    expose_event.xexpose.window = window_id;
    expose_event.xexpose.count = 0;
    XSendEvent(handlers.display, window_id, False, ExposureMask, &expose_event);
  }
} // namespace ymwm::environment
