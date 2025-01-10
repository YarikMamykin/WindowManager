#include "common/Color.h"
#include "config/Layout.h"
#include "config/Window.h"
#include "layouts/Layout.h"
#include "layouts/Parameters.h"
#include "window/Window.h"

#include <format>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <variant>

static inline std::string
window_to_string(const ymwm::window::Window& w) noexcept {
  return std::format(
      "id: {}; x: {}; y: {}; w: {}; h: {}; bw: {}; bc: {},{},{}\n",
      w.id,
      w.x,
      w.y,
      w.w,
      w.h,
      w.border_width,
      w.border_color.red,
      w.border_color.green,
      w.border_color.blue);
}

static inline const ymwm::common::Color color(0xff, 0x0, 0x0);

TEST(TestLayouts, MaximisedLayout) {
  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 0u;
  basic_parameters.screen_margins.right = 0u;
  basic_parameters.screen_margins.top = 0u;
  basic_parameters.screen_margins.bottom = 0u;

  auto parameters = ymwm::layouts::Maximised{};

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

  auto parameters = ymwm::layouts::Maximised{};

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

  auto parameters = ymwm::layouts::Grid(test_windows.size());

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

  auto parameters = ymwm::layouts::Grid(test_windows.size());

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

  ymwm::config::layouts::grid::grid_margins.horizontal = 10u;
  ymwm::config::layouts::grid::grid_margins.vertical = 20u;
  auto parameters = ymwm::layouts::Grid(test_windows.size());

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

TEST(TestLayouts, StackVerticalRight) {
  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 7ul;

  auto parameters =
      ymwm::layouts::StackVerticalRight(basic_parameters.screen_margins,
                                        basic_parameters.screen_width,
                                        basic_parameters.screen_height,
                                        basic_parameters.number_of_windows);

  std::vector<ymwm::window::Window> test_windows(
      basic_parameters.number_of_windows,
      ymwm::window::Window{
          .x = 0,
          .y = 0,
          .w = 200,
          .h = 200,
          .border_width = ymwm::config::windows::regular_border_width,
          .border_color = ymwm::config::windows::regular_border_color });
  ASSERT_EQ(basic_parameters.number_of_windows, test_windows.size());

  ASSERT_EQ(10, basic_parameters.screen_margins.left);
  ASSERT_EQ(10, basic_parameters.screen_margins.right);
  ASSERT_EQ(10, basic_parameters.screen_margins.top);
  ASSERT_EQ(10, basic_parameters.screen_margins.bottom);
  ASSERT_EQ(50, ymwm::config::layouts::stack_vertical_right::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical_right::main_window_margin);
  ASSERT_EQ(5,
            ymwm::config::layouts::stack_vertical_right::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 483,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 502,
                           .y = 10,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 502,
                           .y = 174,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 502,
                           .y = 338,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 502,
                           .y = 502,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 502,
                           .y = 666,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 502,
                           .y = 830,
                           .w = 483,
                           .h = 156,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  ASSERT_EQ(7ul, expected_windows.size());

  auto prepared_layout = ymwm::layouts::Layout(basic_parameters, parameters);

  for (auto& w : test_windows) {
    prepared_layout.apply(w);
  }

  EXPECT_EQ(test_windows, expected_windows)
      << [&test_windows]() -> std::string {
    std::string result;
    for (const auto& w : test_windows) {
      result += window_to_string(w);
    }
    return result;
  }();
}

TEST(TestLayouts, GetLayoutParametersFromString) {
  auto maximised_parameters =
      ymwm::layouts::try_find_parameters(ymwm::layouts::Maximised::type);
  ASSERT_TRUE(maximised_parameters);
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::Maximised>(*maximised_parameters));

  auto grid_parameters =
      ymwm::layouts::try_find_parameters(ymwm::layouts::Grid::type);
  ASSERT_TRUE(grid_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::Grid>(*grid_parameters));

  auto stack_vertical_right_parameters = ymwm::layouts::try_find_parameters(
      ymwm::layouts::StackVerticalRight::type);
  ASSERT_TRUE(stack_vertical_right_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::StackVerticalRight>(
      *stack_vertical_right_parameters));
}

TEST(TestLayouts, GetListOfLayoutsParameters) {
  auto parameters_list = ymwm::layouts::list_of_parameters();
  EXPECT_THAT(parameters_list,
              testing::ElementsAre(ymwm::layouts::Maximised::type,
                                   ymwm::layouts::Grid::type,
                                   ymwm::layouts::StackVerticalRight::type));
}
