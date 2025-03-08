#pragma once

#include "window/Window.h"

#include <gmock/gmock.h>

struct Handlers {};

namespace ymwm::environment {
  using WidthAndHeight = std::tuple<int, int>;

  struct EnvironmentInterface {
    virtual void update_window(const window::Window& w) noexcept = 0;
    virtual void open_window(const window::Window& w) noexcept = 0;
    virtual void focus_window(const window::Window& w) noexcept = 0;
    virtual void reset_focus() noexcept = 0;
    virtual void update_window_border(const window::Window& w) noexcept = 0;
    virtual void change_border_color(const window::Window& w) noexcept = 0;
    virtual void move_and_resize(const window::Window& w) noexcept = 0;
    virtual void close_window(const window::Window& w) noexcept = 0;
    virtual WidthAndHeight screen_width_and_height() noexcept = 0;
  };

  struct TestEnvironment : public EnvironmentInterface {
    MOCK_METHOD(void,
                update_window,
                (const window::Window& w),
                (noexcept, override));
    MOCK_METHOD(void,
                focus_window,
                (const window::Window& w),
                (noexcept, override));
    MOCK_METHOD(void,
                open_window,
                (const window::Window& w),
                (noexcept, override));
    MOCK_METHOD(void, reset_focus, (), (noexcept, override));
    MOCK_METHOD(void,
                update_window_border,
                (const window::Window& w),
                (noexcept, override));
    MOCK_METHOD(void,
                change_border_color,
                (const window::Window& w),
                (noexcept, override));
    MOCK_METHOD(void,
                move_and_resize,
                (const window::Window& w),
                (noexcept, override));
    MOCK_METHOD(void,
                close_window,
                (const window::Window& w),
                (noexcept, override));
    MOCK_METHOD(WidthAndHeight,
                screen_width_and_height,
                (),
                (noexcept, override));
  };
} // namespace ymwm::environment
