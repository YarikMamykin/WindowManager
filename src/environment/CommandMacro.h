#pragma once
#include <string_view>

#define DEFINE_MEMBER(x) x;

#define START_DEFINE_COMMAND(name)                                             \
  struct name {                                                                \
    static inline constexpr std::string_view type{ #name };                    \
    inline constexpr auto operator<=>(const name&) const noexcept = default;   \
    void execute([[maybe_unused]] Environment&) const;

// clang-format off
#define END_DEFINE_COMMAND };
// clang-format on

#define DEFINE_COMMAND(name)                                                   \
  START_DEFINE_COMMAND(name)                                                   \
  END_DEFINE_COMMAND

#define DEFINE_COMMAND_WITH_PARAMS_1(name, member)                             \
  START_DEFINE_COMMAND(name)                                                   \
  DEFINE_MEMBER(member)                                                        \
  END_DEFINE_COMMAND

#define DEFINE_COMMAND_WITH_PARAMS_2(name, member1, member2)                   \
  START_DEFINE_COMMAND(name)                                                   \
  DEFINE_MEMBER(member1)                                                       \
  DEFINE_MEMBER(member2)                                                       \
  END_DEFINE_COMMAND

#define DEFINE_COMMAND_WITH_PARAMS_3(name, member1, member2, member3)          \
  START_DEFINE_COMMAND(name)                                                   \
  DEFINE_MEMBER(member1)                                                       \
  DEFINE_MEMBER(member2)                                                       \
  DEFINE_MEMBER(member3)                                                       \
  END_DEFINE_COMMAND
