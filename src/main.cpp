#include "config/Parser.h"
#include "environment/Command.h"
#include "environment/Environment.h"
#include "events/Map.h"

#include <argparse/argparse.hpp>
#include <stdexcept>
#include <variant>

ymwm::events::Map parse_args(int argc, char** argv) {
  argparse::ArgumentParser arguments_parser("WindowManager");

  arguments_parser.add_argument("--config", "-c")
      .help("Configuration file")
      .default_value(std::string());

  try {
    arguments_parser.parse_args(argc, argv);

    auto config_filepath = arguments_parser.get<std::string>("--config");

    if (not config_filepath.empty()) {
      return ymwm::config::Parser(config_filepath).event_map();
    }
  } catch (const std::runtime_error& err) {
    // No need to fail start, just use default
    // values if failed to parse.
    std::cout << err.what() << std::endl;
  }

  return ymwm::events::default_event_map();
}

int main(int argc, char** argv) {
  ymwm::events::Map events_map = parse_args(argc, argv);

  ymwm::environment::Environment env{ events_map };

  for (auto event = env.event(); not env.exit_requested();
       event = env.event()) {
    if (not events_map.contains(event)) {
      continue;
    }
    env.execute(events_map.at(event), event);
  }

  return 0;
}
