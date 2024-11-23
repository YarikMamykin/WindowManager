#pragma once

#include "Event.h"

#include <cstddef>

namespace ymwm::events {
  struct Hash {
    inline std::size_t operator()(const AbstractKeyPress& e) const noexcept {
      std::size_t hash{ static_cast<std::size_t>(e.mask) };
      return (hash << sizeof(e.mask)) | static_cast<std::size_t>(e.code);
    }

    inline std::size_t operator()(const AbstractMousePress& e) const noexcept {
      std::size_t hash{ static_cast<std::size_t>(e.mask) };
      return (hash << sizeof(e.mask)) | static_cast<std::size_t>(e.mcode);
    }
  };

  struct Hasher {
    inline constexpr std::size_t operator()(const Event& e) const noexcept {
      return std::visit(Hash{}, e);
    }
  };
} // namespace ymwm::events
