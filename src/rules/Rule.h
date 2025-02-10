#pragma once

#include "RuleMacros.h"
#include "environment/Environment.h"
#include "events/Event.h"
#include "layouts/Centered.h"
#include "layouts/Parameters.h"

#include <optional>
#include <variant>

namespace ymwm::rules {

  DEFINE_RULE(FocusedWindowMustBePresent,
              Overridable::No,
              return env.manager().focus().window().has_value();)

  DEFINE_RULE(CurrentLayoutMustBeOneOfStack,
              Overridable::No,
              auto layout_type = env.manager().layout().current();
              return layouts::is_stack_layout(layout_type);)

  DEFINE_RULE(CurrentLayoutMustBeOneOfStackOrCentered,
              Overridable::No,
              auto layout_type = env.manager().layout().current();
              return layouts::is_stack_layout(layout_type) or
                     layout_type == layouts::Centered::type;)

  DEFINE_RULE(CurrentLayoutMustBeGrid,
              Overridable::No,
              auto layout_type = env.manager().layout().current();
              return layout_type == layouts::Grid::type;)

  DEFINE_RULE(AtLeastTwoWindowsPresent,
              Overridable::No,
              return 2ul <= env.manager().windows().size();)

  DEFINE_RULE(EventMustBeWindowAdded,
              Overridable::No,
              return std::holds_alternative<ymwm::events::WindowAdded>(event);)

  DEFINE_RULE(
      EventMustBeWindowNameUpdated,
      Overridable::No,
      return std::holds_alternative<ymwm::events::WindowNameUpdated>(event);)

  DEFINE_RULE(
      EventMustBeMouseOverWindow,
      Overridable::No,
      return std::holds_alternative<ymwm::events::MouseOverWindow>(event);)

  DEFINE_RULE(
      EventMustBeWindowRemoved,
      Overridable::No,
      return std::holds_alternative<ymwm::events::WindowRemoved>(event);)

  using Rule = std::variant<FocusedWindowMustBePresent,
                            CurrentLayoutMustBeOneOfStack,
                            CurrentLayoutMustBeOneOfStackOrCentered,
                            CurrentLayoutMustBeGrid,
                            AtLeastTwoWindowsPresent,
                            EventMustBeWindowAdded,
                            EventMustBeWindowNameUpdated,
                            EventMustBeMouseOverWindow,
                            EventMustBeWindowRemoved>;

  template <std::size_t Index = std::variant_size_v<Rule> - 1ul>
  static inline std::optional<Rule>
  try_find_rule(std::string_view rule_type) noexcept {
    if constexpr (0ul <= Index) {
      using RuleType = std::variant_alternative_t<Index, Rule>;
      if (rule_type == RuleType::type) {
        return RuleType{};
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
} // namespace ymwm::rules
