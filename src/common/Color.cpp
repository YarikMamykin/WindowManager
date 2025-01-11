#include "Color.h"

#include <charconv>

namespace ymwm::common {

  Color::Color() noexcept
      : red(0x0)
      , green(0x0)
      , blue(0x0) {}

  Color::Color(unsigned short r, unsigned short g, unsigned short b) noexcept
      : red(r)
      , green(g)
      , blue(b) {}

  Color::Color(unsigned long color) noexcept {
    unsigned short* component = reinterpret_cast<unsigned short*>(&color);
    blue = *component;
    green = *(component + 1);
    red = *(component + 2);
  }

  Color::Color(const std::string_view color) noexcept {

    if (color.length() < 6) {
      return;
    }

    // Remove '#' if present
    const std::string_view color_view =
        '#' == color.at(0) ? color.substr(1) : color;

    // Convert hex to decimal values
    std::from_chars(color_view.data(), color_view.data() + 2, red, 16);
    std::from_chars(color_view.data() + 2, color_view.data() + 4, green, 16);
    std::from_chars(color_view.data() + 4, color_view.data() + 6, blue, 16);
  }

  Color::operator std::size_t() const noexcept {
    static constexpr auto size_of_component{ sizeof(unsigned short) * 8 };

    unsigned long rgb{ red };
    rgb = rgb << size_of_component;
    rgb |= green;
    rgb = rgb << size_of_component;
    rgb |= blue;

    return rgb;
  }
} // namespace ymwm::common
