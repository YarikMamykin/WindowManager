#pragma once
#include <string_view>

namespace ymwm::common {
  struct Color {
    unsigned short red;
    unsigned short green;
    unsigned short blue;

    Color() noexcept;
    Color(unsigned short r, unsigned short g, unsigned short b) noexcept;
    Color(unsigned long color) noexcept;
    Color(const std::string_view color) noexcept;
    operator std::size_t() const noexcept;

    inline constexpr auto
    operator<=>(const Color& other) const noexcept = default;
  };

  struct ColorHash {
    inline std::size_t operator()(const Color& c) const noexcept {
      return static_cast<std::size_t>(c);
    }
  };
} // namespace ymwm::common
