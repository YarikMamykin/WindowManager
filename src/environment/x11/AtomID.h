#pragma once
#include <cstddef>

namespace ymwm::environment {
  enum AtomID {
    NetWMName,
    Utf8String,
    Clipboard,
    Timestamp,
    Targets,
    ScreenshotPngImage,
    ScreenshotAppQtImage,
    ScreenshotPathsList,
    ScreenshotPath,
    NetActiveWindow,
    DeleteWindow,
    Protocols
  };

  static constexpr inline std::size_t AtomIDSize{ 12 };
} // namespace ymwm::environment
