#pragma once

#include <string_view>

namespace ymwm::environment {
  struct Environment;
}

namespace ymwm::rules {

  enum class Overridable { Yes, No };

  consteval bool is_overridable(Overridable flag) noexcept {
    return flag == Overridable::Yes;
  }

#define START_DEFINE_RULE(name, overridable_flag, default_behavior)            \
  struct name {                                                                \
    static inline constexpr std::string_view type{ #name };                    \
    static inline constexpr Overridable overridable{ is_overridable(           \
        overridable_flag) };                                                   \
    constexpr bool operator()(                                                 \
        [[maybe_unused]] const auto& cmd,                                      \
        [[maybe_unused]] const environment::Environment& env) const noexcept { \
      default_behavior                                                         \
    }                                                                          \
    constexpr auto inline operator<=>(const name&) const noexcept = default;
#define END_DEFINE_RULE                                                        \
  }                                                                            \
  ;

#define DEFINE_RULE(name, overridable_flag, default_behavior)                  \
  START_DEFINE_RULE(name, overridable_flag, default_behavior)                  \
  END_DEFINE_RULE

#define DEFINE_RULE_MEMBER(x) static inline x;
#define DEFINE_RULE_WITH_PARAMS_1(                                             \
    name, overridable_flag, param1, default_behavior)                          \
  START_DEFINE_RULE(name, overridable_flag, default_behavior)                  \
  DEFINE_MEMBER(param1)                                                        \
  END_DEFINE_RULE
} // namespace ymwm::rules
