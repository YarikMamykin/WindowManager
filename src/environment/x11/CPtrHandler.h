#pragma once
#include <cstdlib>

namespace ymwm::environment {
  template <typename Ptr>
  struct CPtrWrapper {
    Ptr ptr = nullptr;
    inline ~CPtrWrapper() noexcept {
      if (ptr) {
        std::free(ptr);
      }
    }
  };
} // namespace ymwm::environment
