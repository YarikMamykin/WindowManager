#include "common/Color.h"
#include "layouts/Layout.h"
#include "layouts/Parameters.h"
#include "window/Window.h"

#include <format>
#include <gtest/gtest.h>

static inline const ymwm::common::Color color(0xff, 0x0, 0x0);

TEST(TestLayouts, MaximisedLayout) {
  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 0u;
  basic_parameters.screen_margins.right = 0u;
  basic_parameters.screen_margins.top = 0u;
  basic_parameters.screen_margins.bottom = 0u;

  auto parameters = ymwm::layouts::MaximisedParameters{};

  std::vector<ymwm::window::Window> test_windows(
      4,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(4ul, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows(
      4,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 998,
                            .h = 998,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(4ul, expected_windows.size());

  auto layout = ymwm::layouts::Layout(basic_parameters, parameters);

  for (auto& w : test_windows) {
    layout.apply(w);
  }

  EXPECT_EQ(test_windows, expected_windows)
      << [&test_windows]() -> std::string {
    std::string result;
    for (const auto& w : test_windows) {
      result += std::format("{} {} {} {}\n", w.x, w.y, w.w, w.h);
    }
    return result;
  }();
}

TEST(TestLayouts, MaximisedLayoutWithScreenMargins) {
  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 1u;
  basic_parameters.screen_margins.right = 1u;
  basic_parameters.screen_margins.top = 1u;
  basic_parameters.screen_margins.bottom = 1u;

  auto parameters = ymwm::layouts::MaximisedParameters{};

  std::vector<ymwm::window::Window> test_windows(
      4,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(4ul, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows(
      4,
      ymwm::window::Window{ .x = 1,
                            .y = 1,
                            .w = 996,
                            .h = 996,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(4ul, expected_windows.size());

  auto layout = ymwm::layouts::Layout(basic_parameters, parameters);

  for (auto& w : test_windows) {
    layout.apply(w);
  }

  EXPECT_EQ(test_windows, expected_windows)
      << [&test_windows]() -> std::string {
    std::string result;
    for (const auto& w : test_windows) {
      result += std::format("{} {} {} {}\n", w.x, w.y, w.w, w.h);
    }
    return result;
  }();
}

TEST(TestLayouts, GridLayout) {
  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 0u;
  basic_parameters.screen_margins.right = 0u;
  basic_parameters.screen_margins.top = 0u;
  basic_parameters.screen_margins.bottom = 0u;
  basic_parameters.number_of_windows = 4ul;
  basic_parameters.focused_window_index = 0ul;

  std::vector<ymwm::window::Window> test_windows(
      4,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(4ul, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{   .x = 0,
                           .y = 0,
                           .w = 498,
                           .h = 498,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 500,
                           .y = 0,
                           .w = 498,
                           .h = 498,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{   .x = 0,
                           .y = 500,
                           .w = 498,
                           .h = 498,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 500,
                           .y = 500,
                           .w = 498,
                           .h = 498,
                           .border_width = 1,
                           .border_color = color }
  });
  ASSERT_EQ(4ul, expected_windows.size());

  auto parameters = ymwm::layouts::GridParameters(
      ymwm::layouts::GridParameters::Margins{ .horizontal = 0u,
                                              .vertical = 0u },
      test_windows.size());

  auto prepared_layout = ymwm::layouts::Layout(basic_parameters, parameters);

  for (auto& w : test_windows) {
    prepared_layout.apply(w);
  }

  EXPECT_EQ(test_windows, expected_windows)
      << [&test_windows]() -> std::string {
    std::string result;
    for (const auto& w : test_windows) {
      result += std::format("{} {} {} {}\n", w.x, w.y, w.w, w.h);
    }
    return result;
  }();
}

TEST(TestLayouts, GridLayout_WithScreenMargins) {
  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 5u;
  basic_parameters.screen_margins.right = 5u;
  basic_parameters.screen_margins.top = 5u;
  basic_parameters.screen_margins.bottom = 5u;
  basic_parameters.number_of_windows = 4ul;
  basic_parameters.focused_window_index = 0ul;

  std::vector<ymwm::window::Window> test_windows(
      4,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(4ul, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{   .x = 5,
                           .y = 5,
                           .w = 493,
                           .h = 493,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 500,
                           .y = 5,
                           .w = 493,
                           .h = 493,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{   .x = 5,
                           .y = 500,
                           .w = 493,
                           .h = 493,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 500,
                           .y = 500,
                           .w = 493,
                           .h = 493,
                           .border_width = 1,
                           .border_color = color }
  });
  ASSERT_EQ(4ul, expected_windows.size());

  auto parameters = ymwm::layouts::GridParameters(
      ymwm::layouts::GridParameters::Margins{ .horizontal = 0u,
                                              .vertical = 0u },
      test_windows.size());

  auto prepared_layout = ymwm::layouts::Layout(basic_parameters, parameters);

  for (auto& w : test_windows) {
    prepared_layout.apply(w);
  }

  EXPECT_EQ(test_windows, expected_windows)
      << [&test_windows]() -> std::string {
    std::string result;
    for (const auto& w : test_windows) {
      result += std::format("{} {} {} {}\n", w.x, w.y, w.w, w.h);
    }
    return result;
  }();
}

TEST(TestLayouts, GridLayout_WithScreenMargins_AndGridMargins) {
  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 5u;
  basic_parameters.screen_margins.right = 5u;
  basic_parameters.screen_margins.top = 5u;
  basic_parameters.screen_margins.bottom = 5u;
  basic_parameters.number_of_windows = 4ul;
  basic_parameters.focused_window_index = 0ul;

  std::vector<ymwm::window::Window> test_windows(
      4,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(4ul, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{   .x = 5,
                           .y = 5,
                           .w = 488,
                           .h = 483,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 505,
                           .y = 5,
                           .w = 488,
                           .h = 483,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{   .x = 5,
                           .y = 510,
                           .w = 488,
                           .h = 483,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 505,
                           .y = 510,
                           .w = 488,
                           .h = 483,
                           .border_width = 1,
                           .border_color = color }
  });
  ASSERT_EQ(4ul, expected_windows.size());

  auto parameters = ymwm::layouts::GridParameters(
      ymwm::layouts::GridParameters::Margins{ .horizontal = 10u,
                                              .vertical = 20u },
      test_windows.size());

  auto prepared_layout = ymwm::layouts::Layout(basic_parameters, parameters);

  for (auto& w : test_windows) {
    prepared_layout.apply(w);
  }

  EXPECT_EQ(test_windows, expected_windows)
      << [&test_windows]() -> std::string {
    std::string result;
    for (const auto& w : test_windows) {
      result += std::format("{} {} {} {}\n", w.x, w.y, w.w, w.h);
    }
    return result;
  }();
}
