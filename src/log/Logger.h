#pragma once
#include <filesystem>
#include <memory>
#include <optional>

namespace ymwm::log {
  enum class LogLevel { Info, Error };

  struct Logger {

    static std::shared_ptr<Logger> create(
        LogLevel log_lvl = LogLevel::Info,
        std::optional<std::filesystem::path>&& logpath = std::nullopt) noexcept;

    static void info(std::string&& s) noexcept;
    static void error(std::string&& s) noexcept;

  private:
    static std::shared_ptr<Logger> m_self_ptr;
  };
} // namespace ymwm::log
