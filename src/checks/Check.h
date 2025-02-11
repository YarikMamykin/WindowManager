#pragma once

#include "CheckMacros.h"
#include "environment/Environment.h"
#include "events/Event.h"
#include "layouts/Centered.h"
#include "layouts/Parameters.h"

#include <optional>
#include <variant>

namespace ymwm::checks {

  DEFINE_CHECK(FocusedWindowMustBePresent,
               Overridable::No,
               return env.manager().focus().window().has_value();)

  DEFINE_CHECK(CurrentLayoutMustBeOneOfStack,
               Overridable::No,
               auto layout_type = env.manager().layout().current();
               return layouts::is_stack_layout(layout_type);)

  DEFINE_CHECK(CurrentLayoutMustBeOneOfStackOrCentered,
               Overridable::No,
               auto layout_type = env.manager().layout().current();
               return layouts::is_stack_layout(layout_type) or
                      layout_type == layouts::Centered::type;)

  DEFINE_CHECK(CurrentLayoutMustBeGrid,
               Overridable::No,
               auto layout_type = env.manager().layout().current();
               return layout_type == layouts::Grid::type;)

  DEFINE_CHECK(AtLeastTwoWindowsPresent,
               Overridable::No,
               return 2ul <= env.manager().windows().size();)

  DEFINE_CHECK(EventMustBeWindowAdded,
               Overridable::No,
               return std::holds_alternative<ymwm::events::WindowAdded>(event);)

  DEFINE_CHECK(
      EventMustBeWindowNameUpdated,
      Overridable::No,
      return std::holds_alternative<ymwm::events::WindowNameUpdated>(event);)

  DEFINE_CHECK(
      EventMustBeMouseOverWindow,
      Overridable::No,
      return std::holds_alternative<ymwm::events::MouseOverWindow>(event);)

  DEFINE_CHECK(
      EventMustBeWindowRemoved,
      Overridable::No,
      return std::holds_alternative<ymwm::events::WindowRemoved>(event);)

  using Check = std::variant<FocusedWindowMustBePresent,
                             CurrentLayoutMustBeOneOfStack,
                             CurrentLayoutMustBeOneOfStackOrCentered,
                             CurrentLayoutMustBeGrid,
                             AtLeastTwoWindowsPresent,
                             EventMustBeWindowAdded,
                             EventMustBeWindowNameUpdated,
                             EventMustBeMouseOverWindow,
                             EventMustBeWindowRemoved>;

  template <std::size_t Index = std::variant_size_v<Check> - 1ul>
  static inline std::optional<Check>
  try_find_rule(std::string_view rule_type) noexcept {
    if constexpr (0ul <= Index) {
      using CheckType = std::variant_alternative_t<Index, Check>;
      if (rule_type == CheckType::type) {
        return CheckType{};
      }

      if constexpr (0ul == Index) {
        return std::nullopt;
      } else {
        return try_find_rule<Index - 1ul>(rule_type);
      }

    } else {
      return std::nullopt;
    }
  }
} // namespace ymwm::checks
