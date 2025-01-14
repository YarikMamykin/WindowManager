#pragma once

#include <algorithm>

namespace ymwm::common {
  template <unsigned int min = 0u, unsigned int max = 100u>
  struct Ratio {

    inline Ratio(unsigned int value) noexcept
        : m_value(std::clamp(value, min, max)) {}

    inline operator unsigned int() const noexcept { return m_value; }

  private:
    int m_value;
  };
} // namespace ymwm::common
