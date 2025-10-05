#pragma once
#include <cstddef>

namespace ymwm::environment {
  enum AtomID {
    NetWMName,
    Utf8String,
    Clipboard,
    Targets,
    ScreenshotImage,
    ScreenshotPathsList,
    ScreenshotPath,
    NetActiveWindow,
    DeleteWindow,
    Protocols
  };

  static constexpr inline std::size_t AtomIDSize{ 10 };
} // namespace ymwm::environment
