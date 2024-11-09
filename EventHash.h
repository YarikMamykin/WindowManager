#pragma once

#include "Event.h"

#include <cstddef>

namespace ymwm::events {
  struct EventHash {
    inline std::size_t operator()(const AbstractKeyPress& e) const noexcept{
      std::size_t hash{ static_cast<std::size_t>(e.mask) };
      return (hash << sizeof(e.mask)) | static_cast<std::size_t>(e.code);
    }

    inline std::size_t operator()(const AbstractMousePress& e) const noexcept{
      std::size_t hash{ static_cast<std::size_t>(e.mask) };
      return (hash << sizeof(e.mask)) | static_cast<std::size_t>(e.mcode);
    }
  };

  struct EventHasher {
    inline constexpr std::size_t operator()(const Event& e) const noexcept {
      return std::visit(EventHash{}, e);
    }
  };
} // namespace ymwm::events
