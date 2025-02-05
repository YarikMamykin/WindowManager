#pragma once
#include <cstdlib>

namespace ymwm::environment {
  template <typename T>
  concept ToBeXkbRF_VarDefsRec = requires(T v) {
    v.model;
    v.layout;
    v.variant;
    v.options;
  };

  // Making template concept allows decoupling from actual type
  // and not to break X11 include headers order.
  template <typename T>
    requires ToBeXkbRF_VarDefsRec<T>
  struct XkbRF_VarDefsRecWrapper {
    T v;
    ~XkbRF_VarDefsRecWrapper() noexcept {
      if (v.model) {
        std::free(v.model);
      }
      if (v.variant) {
        std::free(v.variant);
      }
      if (v.options) {
        std::free(v.options);
      }
    }
  };
} // namespace ymwm::environment
