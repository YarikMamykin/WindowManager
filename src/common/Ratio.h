#pragma once

#include <algorithm>

namespace ymwm::common {
  struct Ratio {

    inline Ratio(unsigned int value) noexcept
        : m_value(std::clamp(value, min, max)) {}

    inline operator unsigned int() const noexcept { return m_value; }

  private:
    static constexpr inline unsigned int max{ 100 };
    static constexpr inline unsigned int min{ 0 };
    int m_value;
  };
} // namespace ymwm::common
