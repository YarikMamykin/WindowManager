#pragma once
#include <cstddef>

namespace ymwm::environment {
  enum AtomID {
    NetWMName,
    Utf8String,
    Clipboard,
    Timestamp,
    Targets,
    ScreenshotImage,
    ScreenshotPathsList,
    ScreenshotPath,
    NetActiveWindow,
    DeleteWindow,
    Protocols
  };

  static constexpr inline std::size_t AtomIDSize{ 11 };
} // namespace ymwm::environment
