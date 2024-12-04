#pragma once

#include <string_view>
#include <variant>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::environment::commands {
  struct Dummy {
    static inline constexpr std::string_view type{ "Dummy" };
    void execute([[maybe_unused]] Environment&) const;
  };

  struct ExitRequested {
    static inline constexpr std::string_view type{ "ExitRequested" };
    void execute([[maybe_unused]] Environment&) const;
  };

  using Command = std::variant<Dummy, ExitRequested>;
} // namespace ymwm::environment::commands
