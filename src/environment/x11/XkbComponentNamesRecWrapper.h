#pragma once
#include <cstdlib>

namespace ymwm::environment {
  template <typename T>
  concept ToBeXkbComponentNamesRec = requires(T v) {
    v.keymap;
    v.keycodes;
    v.types;
    v.compat;
    v.symbols;
    v.geometry;
  };

  // Making template concept allows decoupling from actual type
  // and not to break X11 include headers order.
  template <typename T>
    requires ToBeXkbComponentNamesRec<T>
  struct XkbComponentNamesRecWrapper {
    T v;
    ~XkbComponentNamesRecWrapper() noexcept {
      if (v.compat) {
        std::free(v.compat);
      }
      if (v.geometry) {
        std::free(v.geometry);
      }
      if (v.keycodes) {
        std::free(v.keycodes);
      }
      if (v.keymap) {
        std::free(v.keymap);
      }
      if (v.symbols) {
        std::free(v.symbols);
      }
      if (v.types) {
        std::free(v.types);
      }
    }
  };
} // namespace ymwm::environment
