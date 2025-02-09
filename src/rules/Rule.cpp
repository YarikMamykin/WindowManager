#include "Rule.h"

#include "RuleApi.h"
#include "environment/Environment.h"
#include "rules/Map.h"

#include <iostream>
#include <string_view>
#include <variant>

namespace ymwm::rules {
  bool passes_rules(const environment::commands::Command& cmd,
                    const environment::Environment& env) noexcept {
    return std::visit(
        [&env](const auto& c) -> bool {
          if (not rules_map.contains(c.type)) {
            // Command doesn't have specific rules, no check required.
            return true;
          }

          const auto& rules = rules_map.at(c.type);
          for (std::string_view rule_type : rules) {
            auto rule = try_find_rule(rule_type);
            if (not rule) {
              std::cerr << "Invalid rule type: " << rule_type << "!\n";
              // If rule type not found, then just skip checks.
              // No need to exit with fail entirely.
              return true;
            }

            auto visitor = [&c, &env](const auto& rule) -> bool {
              return rule(c, env);
            };

            if (bool rule_passed = std::visit(visitor, *rule);
                not rule_passed) {
              std::cerr << "Rule " << rule_type << " didn't pass for command "
                        << c.type << "\n";
              return rule_passed;
            }
          }

          return true;
        },
        cmd);
  }
} // namespace ymwm::rules
