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

    inline std::size_t operator()(const AbstractUnknownEvent&) const noexcept {
      return 0u;
    }

    inline std::size_t operator()(const WindowAdded& e) const noexcept {
      return std::hash<std::string_view>{}(e.type);
    }

    inline std::size_t operator()(const WindowRemoved& e) const noexcept {
      return std::hash<std::string_view>{}(e.type);
    }

    inline std::size_t operator()(const WindowNameUpdated& e) const noexcept {
      return std::hash<std::string_view>{}(e.type);
    }

    inline std::size_t operator()(const MouseOverWindow& e) const noexcept {
      return std::hash<std::string_view>{}(e.type);
    }
  };

  struct Hasher {
    inline constexpr std::size_t operator()(const Event& e) const noexcept {
      return std::visit(Hash{}, e);
    }
  };
} // namespace ymwm::events
