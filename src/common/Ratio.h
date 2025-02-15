#pragma once

#include <algorithm>

namespace ymwm::common {
  template <unsigned int min = 0u, unsigned int max = 100u>
  struct Ratio {
    using UnderlyingType = unsigned int;

    inline Ratio(UnderlyingType value) noexcept
        : m_value(std::clamp(value, min, max)) {}

    inline operator UnderlyingType() const noexcept { return m_value; }

  private:
    UnderlyingType m_value;
  };
} // namespace ymwm::common
