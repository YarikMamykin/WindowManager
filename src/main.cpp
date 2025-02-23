#include "args/Parser.h"
#include "config/Parser.h"
#include "environment/Environment.h"

#include <stdexcept>

ymwm::events::Map parse_config(std::filesystem::path&& config_path);

int main(int argc, char** argv) {

  auto parsed_args = ymwm::args::parse(argc, argv);
  if (not parsed_args.generated_default_config_path.empty()) {
    ymwm::config::Parser::default_config_to_yaml(
        std::move(parsed_args.generated_default_config_path));
    return 0;
  }

  auto events_map = parse_config(std::move(parsed_args.config_path));

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

ymwm::events::Map parse_config(std::filesystem::path&& config_path) {
  try {
    return ymwm::config::Parser(std::move(config_path)).event_map();
  } catch (const std::runtime_error& err) {
    // No need to fail start, just use default
    // values if failed to parse.
    std::cout << err.what() << std::endl;
  }

  return ymwm::events::default_event_map();
}
