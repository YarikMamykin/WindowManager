#include "args/Parser.h"
#include "config/Parser.h"
#include "environment/Environment.h"

int main(int argc, char** argv) {

  auto parsed_args = ymwm::args::parse(argc, argv);
  if (not parsed_args.generated_default_config_path.empty()) {
    ymwm::config::Parser::default_config_to_yaml(
        std::move(parsed_args.generated_default_config_path));
    return 0;
  }

  auto events_map =
      ymwm::config::parse_config(std::move(parsed_args.config_path));

  ymwm::environment::Environment env{ events_map };
  if (env.exit_requested()) {
    return 1;
  }

  for (auto event = env.event(); not env.exit_requested();
       event = env.event()) {
    if (not events_map.contains(event)) {
      continue;
    }
    env.execute(events_map.at(event), event);
  }

  return 0;
}
