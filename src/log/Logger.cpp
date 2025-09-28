#include "Logger.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace ymwm::log {
  std::shared_ptr<Logger>
  Logger::create(LogLevel log_lvl,
                 std::optional<std::filesystem::path>&& logpath) noexcept {
    if (auto ptr_lock = std::weak_ptr<Logger>(m_self_ptr).lock(); ptr_lock) {
      return m_self_ptr;
    }

    try {

      std::vector<spdlog::sink_ptr> sinks;
      if (logpath) {
        sinks.reserve(3);
        sinks.push_back(
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(
            std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            *logpath, true));
      } else {
        sinks.reserve(2);
        sinks.push_back(
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(
            std::make_shared<spdlog::sinks::stderr_color_sink_mt>());
      }

      // Create logger with both sinks
      auto logger = std::make_shared<spdlog::logger>(
          "ymwm-logger", sinks.begin(), sinks.end());

      // Set as default logger
      spdlog::set_default_logger(logger);

      // Set log level
      spdlog::set_level(log_lvl == LogLevel::Info ? spdlog::level::info
                                                  : spdlog::level::err);
      // Set flush for each minute
      spdlog::flush_every(std::chrono::seconds(60));

    } catch (const spdlog::spdlog_ex& ex) {
      std::cerr << "Log init failed: " << ex.what() << std::endl;
      return nullptr;
    }
    // init here with file or whatever
    m_self_ptr = std::make_shared<Logger>();

    return m_self_ptr;
  }

  void Logger::info(std::string&& s) noexcept { spdlog::info(std::move(s)); }

  void Logger::error(std::string&& s) noexcept { spdlog::error(std::move(s)); }

  std::shared_ptr<Logger> Logger::m_self_ptr;
} // namespace ymwm::log
