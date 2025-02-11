#include "Check.h"
#include "ChecksApi.h"
#include "checks/Map.h"
#include "environment/Environment.h"

#include <iostream>
#include <string_view>
#include <variant>

namespace ymwm::checks {
  bool passes_checks(const environment::commands::Command& cmd,
                     [[maybe_unused]] const ymwm::events::Event& event,
                     const environment::Environment& env) noexcept {
    return std::visit(
        [&env, event](const auto& c) -> bool {
          if (not checks_map.contains(c.type)) {
            // Command doesn't have specific checks, no check required.
            return true;
          }

          const auto& checks = checks_map.at(c.type);
          for (std::string_view rule_type : checks) {
            auto rule = try_find_rule(rule_type);
            if (not rule) {
              std::cerr << "Invalid rule type: " << rule_type << "!\n";
              // If rule type not found, then just skip checks.
              // No need to exit with fail entirely.
              return true;
            }

            auto visitor = [&c, &event, &env](const auto& rule) -> bool {
              return rule(c, event, env);
            };

            if (bool rule_passed = std::visit(visitor, *rule);
                not rule_passed) {
              std::cerr << "Check " << rule_type << " didn't pass for command "
                        << c.type << "\n";
              return rule_passed;
            }
          }

          return true;
        },
        cmd);
  }
} // namespace ymwm::checks
