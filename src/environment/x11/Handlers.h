#pragma once
#include "BackgroundImageHandler.h"
#include "common/Color.h"
#include "environment/x11/AtomID.h"

#include <array>
#include <cstring>
#include <memory>
#include <unordered_map>
// clang-format on

namespace ymwm::environment {
  struct Handlers {
    Display* display;
    int screen;
    Window root_window;
    Cursor cursor;
    Colormap colormap;
    std::unique_ptr<BackgroundImageHandler> background_image;
    std::unordered_map<common::Color, XColor, common::ColorHash> colors;
    std::array<Atom, AtomIDSize> atoms;
    int current_layout;
    int max_layouts;

    Handlers();

    ~Handlers();

    inline bool valid() const noexcept { return static_cast<bool>(display); }

  private:
    int get_number_of_layouts() const noexcept;
  };

} // namespace ymwm::environment
