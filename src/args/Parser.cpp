#include "Parser.h"

#include <argparse/argparse.hpp>

namespace ymwm::args {

  ParsedArgs parse(int argc, char** argv) {
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

} // namespace ymwm::args
