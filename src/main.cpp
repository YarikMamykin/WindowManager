#include "config/Parser.h"
#include "environment/Command.h"
#include "environment/Environment.h"
#include "events/Map.h"

#include <argparse/argparse.hpp>
#include <stdexcept>
#include <variant>

struct ParsedArgs {
  std::filesystem::path generated_default_config_path;
  std::filesystem::path config_path;
};

ParsedArgs parse_args(int argc, char** argv);
ymwm::events::Map parse_config(std::filesystem::path&& config_path);

int main(int argc, char** argv) {

  auto parsed_args = parse_args(argc, argv);
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

ParsedArgs parse_args(int argc, char** argv) {
#ifndef VERSION
#define VERSION "0.1.0"
#endif

  argparse::ArgumentParser arguments_parser("WindowManager", VERSION);

  arguments_parser.add_argument("--config", "-c")
      .help("Configuration file path.")
      .default_value(std::string());
  arguments_parser.add_argument("--generate-default-config-only", "-gdco")
      .help("File path where to generate default config.")
      .default_value(std::string());

  arguments_parser.parse_args(argc, argv);

  return ParsedArgs{
    .generated_default_config_path =
        arguments_parser.get<std::string>("--generate-default-config-only"),
    .config_path = arguments_parser.get<std::string>("--config")
  };
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
