#pragma once

#include <variant>

namespace ymwm::commands {
  struct Dummy {
    static inline constexpr char type[]{ "Dummy" };
    void execute() const;
  };

  struct None {
    static inline constexpr char type[]{ "None" };
    void execute() const;
  };

  using Command = std::variant<Dummy, None>;
} // namespace ymwm::commands
