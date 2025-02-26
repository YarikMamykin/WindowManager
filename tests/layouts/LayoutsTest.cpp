#include "common/Color.h"
#include "common/VariantInterfaceHelpers.h"
#include "config/Layout.h"
#include "config/Window.h"
#include "layouts/Centered.h"
#include "layouts/Helpers.h"
#include "layouts/Layout.h"
#include "layouts/ParallelHorizontal.h"
#include "layouts/ParallelVertical.h"
#include "layouts/Parameters.h"
#include "layouts/StackHorizontalBottom.h"
#include "layouts/StackHorizontalDouble.h"
#include "layouts/StackHorizontalTop.h"
#include "layouts/StackVerticalDouble.h"
#include "window/Window.h"

#include <format>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <initializer_list>
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

static inline void
verify_windows(const ymwm::layouts::Layout::BasicParameters& basic_parameters,
               const ymwm::layouts::Parameters& parameters,
               const std::vector<ymwm::window::Window>& expected_windows,
               std::vector<ymwm::window::Window>& test_windows) noexcept {

  ASSERT_EQ(basic_parameters.number_of_windows, expected_windows.size());

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

TEST(TestLayouts, CenteredLayout) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::centered::window_width_ratio = 50;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 4ul;

  auto parameters = ymwm::layouts::Centered(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      ymwm::config::layouts::centered::window_width_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::centered::window_width_ratio);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows(
      basic_parameters.number_of_windows,
      {
          ymwm::window::Window{
                               .x = 245,
                               .y = 10,
                               .w = 486,
                               .h = 976,
                               .border_width = ymwm::config::windows::regular_border_width,
                               .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, CenteredLayoutWithScreenMargins) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::centered::window_width_ratio = 100;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 1u;
  basic_parameters.screen_margins.right = 1u;
  basic_parameters.screen_margins.top = 1u;
  basic_parameters.screen_margins.bottom = 1u;
  basic_parameters.number_of_windows = 4ul;

  auto parameters = ymwm::layouts::Centered{
    basic_parameters.screen_margins,
    basic_parameters.screen_width,
    basic_parameters.screen_height,
    ymwm::config::layouts::centered::window_width_ratio
  };

  std::vector<ymwm::window::Window> test_windows(
      basic_parameters.number_of_windows,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(basic_parameters.number_of_windows, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows(
      basic_parameters.number_of_windows,
      ymwm::window::Window{ .x = 1,
                            .y = 1,
                            .w = 994,
                            .h = 994,
                            .border_width = 1,
                            .border_color = color });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, GridLayout) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 0u;
  basic_parameters.screen_margins.right = 0u;
  basic_parameters.screen_margins.top = 0u;
  basic_parameters.screen_margins.bottom = 0u;
  basic_parameters.number_of_windows = 4ul;

  std::vector<ymwm::window::Window> test_windows(
      basic_parameters.number_of_windows,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(basic_parameters.number_of_windows, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{   .x = 0,
                           .y = 0,
                           .w = 496,
                           .h = 496,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 500,
                           .y = 0,
                           .w = 496,
                           .h = 496,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{   .x = 0,
                           .y = 500,
                           .w = 496,
                           .h = 496,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 500,
                           .y = 500,
                           .w = 496,
                           .h = 496,
                           .border_width = 1,
                           .border_color = color }
  });
  auto parameters = ymwm::layouts::Grid(test_windows.size());

  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, GridLayout_WithScreenMargins) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 5u;
  basic_parameters.screen_margins.right = 5u;
  basic_parameters.screen_margins.top = 5u;
  basic_parameters.screen_margins.bottom = 5u;
  basic_parameters.number_of_windows = 4ul;

  std::vector<ymwm::window::Window> test_windows(
      basic_parameters.number_of_windows,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(basic_parameters.number_of_windows, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{   .x = 5,
                           .y = 5,
                           .w = 491,
                           .h = 491,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 500,
                           .y = 5,
                           .w = 491,
                           .h = 491,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{   .x = 5,
                           .y = 500,
                           .w = 491,
                           .h = 491,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 500,
                           .y = 500,
                           .w = 491,
                           .h = 491,
                           .border_width = 1,
                           .border_color = color }
  });
  ASSERT_EQ(basic_parameters.number_of_windows, expected_windows.size());

  auto parameters = ymwm::layouts::Grid(test_windows.size());

  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, GridLayout_WithScreenMargins_AndGridMargins) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 5u;
  basic_parameters.screen_margins.right = 5u;
  basic_parameters.screen_margins.top = 5u;
  basic_parameters.screen_margins.bottom = 5u;
  basic_parameters.number_of_windows = 4ul;

  std::vector<ymwm::window::Window> test_windows(
      basic_parameters.number_of_windows,
      ymwm::window::Window{ .x = 0,
                            .y = 0,
                            .w = 200,
                            .h = 200,
                            .border_width = 1,
                            .border_color = color });
  ASSERT_EQ(basic_parameters.number_of_windows, test_windows.size());

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{   .x = 5,
                           .y = 5,
                           .w = 486,
                           .h = 481,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 505,
                           .y = 5,
                           .w = 486,
                           .h = 481,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{   .x = 5,
                           .y = 510,
                           .w = 486,
                           .h = 481,
                           .border_width = 1,
                           .border_color = color },
      ymwm::window::Window{ .x = 505,
                           .y = 510,
                           .w = 486,
                           .h = 481,
                           .border_width = 1,
                           .border_color = color }
  });
  ASSERT_EQ(basic_parameters.number_of_windows, expected_windows.size());

  ymwm::config::layouts::grid::grid_margins.horizontal = 10u;
  ymwm::config::layouts::grid::grid_margins.vertical = 20u;
  auto parameters = ymwm::layouts::Grid(test_windows.size());

  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackVerticalRight) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 7ul;

  auto parameters = ymwm::layouts::StackVerticalRight(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_vertical::main_window_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::stack_vertical::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::main_window_margin);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::stack_window_margin);
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
  ASSERT_EQ(basic_parameters.number_of_windows, expected_windows.size());

  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackVerticalLeft) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 7ul;

  auto parameters = ymwm::layouts::StackVerticalLeft(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_vertical::main_window_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::stack_vertical::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::main_window_margin);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 502,
                           .y = 10,
                           .w = 483,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 174,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 338,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 502,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 666,
                           .w = 483,
                           .h = 155,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 830,
                           .w = 483,
                           .h = 156,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  ASSERT_EQ(basic_parameters.number_of_windows, expected_windows.size());

  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, ParallelVertical) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::parallel::margin = 2;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 4ul;

  auto parameters =
      ymwm::layouts::ParallelVertical(basic_parameters.screen_margins,
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
  ASSERT_EQ(2u, ymwm::config::layouts::parallel::margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 239,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 255,
                           .y = 10,
                           .w = 239,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 500,
                           .y = 10,
                           .w = 239,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 745,
                           .y = 10,
                           .w = 241,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, ParallelHorizontal) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::parallel::margin = 2;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 4ul;

  auto parameters =
      ymwm::layouts::ParallelHorizontal(basic_parameters.screen_margins,
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
  ASSERT_EQ(2u, ymwm::config::layouts::parallel::margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 976,
                           .h = 239,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 255,
                           .w = 976,
                           .h = 239,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 500,
                           .w = 976,
                           .h = 239,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 745,
                           .w = 976,
                           .h = 241,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackHorizontalTop) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_horizontal::main_window_ratio = 50;
  ymwm::config::layouts::stack_horizontal::main_window_margin = 10;
  ymwm::config::layouts::stack_horizontal::stack_window_margin = 10;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 4ul;

  auto parameters = ymwm::layouts::StackHorizontalTop(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_horizontal::main_window_ratio);

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
  ASSERT_EQ(50u, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  ASSERT_EQ(10u, ymwm::config::layouts::stack_horizontal::main_window_margin);
  ASSERT_EQ(10u, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 976,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 505,
                           .w = 314,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 338,
                           .y = 505,
                           .w = 314,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 666,
                           .y = 505,
                           // adjusted width
                           .w = 320,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackHorizontalBottom) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_horizontal::main_window_ratio = 50;
  ymwm::config::layouts::stack_horizontal::main_window_margin = 10;
  ymwm::config::layouts::stack_horizontal::stack_window_margin = 10;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 4ul;

  auto parameters = ymwm::layouts::StackHorizontalBottom(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_horizontal::main_window_ratio);

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
  ASSERT_EQ(50u, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  ASSERT_EQ(10u, ymwm::config::layouts::stack_horizontal::main_window_margin);
  ASSERT_EQ(10u, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 505,
                           .w = 976,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 314,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 338,
                           .y = 10,
                           .w = 314,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 666,
                           .y = 10,
                           // adjusted width
                           .w = 320,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackVerticalDouble) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_vertical::main_window_ratio = 50;
  ymwm::config::layouts::stack_vertical::main_window_margin = 10;
  ymwm::config::layouts::stack_vertical::stack_window_margin = 10;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 5ul;

  auto parameters = ymwm::layouts::StackVerticalDouble(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_vertical::main_window_ratio);

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
  ASSERT_EQ(50u, ymwm::config::layouts::stack_vertical::main_window_ratio);
  ASSERT_EQ(10u, ymwm::config::layouts::stack_vertical::main_window_margin);
  ASSERT_EQ(10u, ymwm::config::layouts::stack_vertical::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 261,
                           .y = 10,
                           .w = 474,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 237,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 749,
                           .y = 10,
                           .w = 237,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 505,
                           .w = 237,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 749,
                           .y = 505,
                           .w = 237,
                           .h = 481,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackHorizontalDouble) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_horizontal::main_window_ratio = 50;
  ymwm::config::layouts::stack_horizontal::main_window_margin = 10;
  ymwm::config::layouts::stack_horizontal::stack_window_margin = 10;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 5ul;

  auto parameters = ymwm::layouts::StackHorizontalDouble(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_horizontal::main_window_ratio);

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
  ASSERT_EQ(50u, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  ASSERT_EQ(10u, ymwm::config::layouts::stack_horizontal::main_window_margin);
  ASSERT_EQ(10u, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 261,
                           .w = 976,
                           .h = 474,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 481,
                           .h = 237,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 10,
                           .y = 749,
                           .w = 481,
                           .h = 237,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 505,
                           .y = 10,
                           .w = 481,
                           .h = 237,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
      ymwm::window::Window{
                           .x = 505,
                           .y = 749,
                           .w = 481,
                           .h = 237,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackVerticalRightLayout_MainWindowOnly) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_vertical::main_window_ratio = 50;
  ymwm::config::layouts::stack_vertical::main_window_margin = 5;
  ymwm::config::layouts::stack_vertical::stack_window_margin = 5;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 1ul;

  auto parameters = ymwm::layouts::StackVerticalRight(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_vertical::main_window_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::stack_vertical::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::main_window_margin);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 976,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackVerticalLeftLayout_MainWindowOnly) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_vertical::main_window_ratio = 50;
  ymwm::config::layouts::stack_vertical::main_window_margin = 5;
  ymwm::config::layouts::stack_vertical::stack_window_margin = 5;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 1ul;

  auto parameters = ymwm::layouts::StackVerticalLeft(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_vertical::main_window_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::stack_vertical::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::main_window_margin);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 976,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackHorizontalTopLayout_MainWindowOnly) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_horizontal::main_window_ratio = 50;
  ymwm::config::layouts::stack_horizontal::main_window_margin = 5;
  ymwm::config::layouts::stack_horizontal::stack_window_margin = 5;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 1ul;

  auto parameters = ymwm::layouts::StackHorizontalTop(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_horizontal::main_window_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_horizontal::main_window_margin);
  ASSERT_EQ(5, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 976,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackHorizontalBottomLayout_MainWindowOnly) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_horizontal::main_window_ratio = 50;
  ymwm::config::layouts::stack_horizontal::main_window_margin = 5;
  ymwm::config::layouts::stack_horizontal::stack_window_margin = 5;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 1ul;

  auto parameters = ymwm::layouts::StackHorizontalBottom(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_horizontal::main_window_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_horizontal::main_window_margin);
  ASSERT_EQ(5, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 976,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackVerticalDoubleLayout_MainWindowOnly) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_vertical::main_window_ratio = 50;
  ymwm::config::layouts::stack_vertical::main_window_margin = 5;
  ymwm::config::layouts::stack_vertical::stack_window_margin = 5;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 1ul;

  auto parameters = ymwm::layouts::StackVerticalDouble(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_vertical::main_window_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::stack_vertical::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::main_window_margin);
  ASSERT_EQ(5, ymwm::config::layouts::stack_vertical::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 976,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, StackHorizontalDoubleLayout_MainWindowOnly) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::layouts::stack_horizontal::main_window_ratio = 50;
  ymwm::config::layouts::stack_horizontal::main_window_margin = 5;
  ymwm::config::layouts::stack_horizontal::stack_window_margin = 5;

  ymwm::layouts::Layout::BasicParameters basic_parameters;
  basic_parameters.screen_width = 1000;
  basic_parameters.screen_height = 1000;
  basic_parameters.screen_margins.left = 10u;
  basic_parameters.screen_margins.right = 10u;
  basic_parameters.screen_margins.top = 10u;
  basic_parameters.screen_margins.bottom = 10u;
  basic_parameters.number_of_windows = 1ul;

  auto parameters = ymwm::layouts::StackHorizontalDouble(
      basic_parameters.screen_margins,
      basic_parameters.screen_width,
      basic_parameters.screen_height,
      basic_parameters.number_of_windows,
      ymwm::config::layouts::stack_horizontal::main_window_ratio);

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
  ASSERT_EQ(50, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  ASSERT_EQ(5, ymwm::config::layouts::stack_horizontal::main_window_margin);
  ASSERT_EQ(5, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  ASSERT_EQ(1000, basic_parameters.screen_width);
  ASSERT_EQ(1000, basic_parameters.screen_height);
  ASSERT_EQ(4, parameters.two_borders);

  std::vector<ymwm::window::Window> expected_windows({
      ymwm::window::Window{
                           .x = 10,
                           .y = 10,
                           .w = 976,
                           .h = 976,
                           .border_width = ymwm::config::windows::regular_border_width,
                           .border_color = ymwm::config::windows::regular_border_color },
  });
  verify_windows(basic_parameters, parameters, expected_windows, test_windows);
}

TEST(TestLayouts, GetLayoutParametersFromString) {
  auto centered_parameters =
      ymwm::common::try_find_type<ymwm::layouts::Parameters>(
          ymwm::layouts::Centered::type);
  ASSERT_TRUE(centered_parameters);
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::Centered>(*centered_parameters));

  auto grid_parameters = ymwm::common::try_find_type<ymwm::layouts::Parameters>(
      ymwm::layouts::Grid::type);
  ASSERT_TRUE(grid_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::Grid>(*grid_parameters));

  auto stack_vertical_parameters =
      ymwm::common::try_find_type<ymwm::layouts::Parameters>(
          ymwm::layouts::StackVerticalRight::type);
  ASSERT_TRUE(stack_vertical_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::StackVerticalRight>(
      *stack_vertical_parameters));

  stack_vertical_parameters =
      ymwm::common::try_find_type<ymwm::layouts::Parameters>(
          ymwm::layouts::StackVerticalLeft::type);
  ASSERT_TRUE(stack_vertical_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::StackVerticalLeft>(
      *stack_vertical_parameters));

  stack_vertical_parameters =
      ymwm::common::try_find_type<ymwm::layouts::Parameters>(
          ymwm::layouts::StackVerticalDouble::type);
  ASSERT_TRUE(stack_vertical_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::StackVerticalDouble>(
      *stack_vertical_parameters));

  auto stack_horizontal_parameters =
      ymwm::common::try_find_type<ymwm::layouts::Parameters>(
          ymwm::layouts::StackHorizontalTop::type);
  ASSERT_TRUE(stack_horizontal_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::StackHorizontalTop>(
      *stack_horizontal_parameters));

  stack_horizontal_parameters =
      ymwm::common::try_find_type<ymwm::layouts::Parameters>(
          ymwm::layouts::StackHorizontalBottom::type);
  ASSERT_TRUE(stack_horizontal_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::StackHorizontalBottom>(
      *stack_horizontal_parameters));

  stack_horizontal_parameters =
      ymwm::common::try_find_type<ymwm::layouts::Parameters>(
          ymwm::layouts::StackHorizontalDouble::type);
  ASSERT_TRUE(stack_horizontal_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::StackHorizontalDouble>(
      *stack_horizontal_parameters));

  auto parallel_parameters =
      ymwm::common::try_find_type<ymwm::layouts::Parameters>(
          ymwm::layouts::ParallelVertical::type);
  ASSERT_TRUE(parallel_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::ParallelVertical>(
      *parallel_parameters));

  parallel_parameters = ymwm::common::try_find_type<ymwm::layouts::Parameters>(
      ymwm::layouts::ParallelHorizontal::type);
  ASSERT_TRUE(parallel_parameters);
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::ParallelHorizontal>(
      *parallel_parameters));
}

TEST(TestLayouts, GetListOfLayoutsParameters) {
  auto parameters_list = ymwm::common::known_ids<ymwm::layouts::Parameters>();
  EXPECT_THAT(parameters_list,
              testing::ElementsAre(ymwm::layouts::Centered::type,
                                   ymwm::layouts::Grid::type,
                                   ymwm::layouts::StackVerticalRight::type,
                                   ymwm::layouts::StackVerticalLeft::type,
                                   ymwm::layouts::StackVerticalDouble::type,
                                   ymwm::layouts::StackHorizontalTop::type,
                                   ymwm::layouts::StackHorizontalBottom::type,
                                   ymwm::layouts::StackHorizontalDouble::type,
                                   ymwm::layouts::ParallelVertical::type,
                                   ymwm::layouts::ParallelHorizontal::type));
}

TEST(TestLayoutsUtils, TestIsLayout) {
  EXPECT_TRUE(
      ymwm::layouts::is<ymwm::layouts::Centered>(ymwm::layouts::Centered{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::Grid>(ymwm::layouts::Grid{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::StackVerticalRight>(
      ymwm::layouts::StackVerticalRight{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::StackVerticalLeft>(
      ymwm::layouts::StackVerticalLeft{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::StackVerticalDouble>(
      ymwm::layouts::StackVerticalDouble{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::StackHorizontalTop>(
      ymwm::layouts::StackHorizontalTop{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::StackHorizontalBottom>(
      ymwm::layouts::StackHorizontalBottom{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::StackHorizontalDouble>(
      ymwm::layouts::StackHorizontalDouble{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::ParallelVertical>(
      ymwm::layouts::ParallelVertical{}));
  EXPECT_TRUE(ymwm::layouts::is<ymwm::layouts::ParallelHorizontal>(
      ymwm::layouts::ParallelHorizontal{}));
}

TEST(TestLayoutsUtils, TestIsStackHorizontalLayout) {
  using namespace ymwm::layouts;
  for (const Parameters& p :
       std::initializer_list<Parameters>{ StackHorizontalBottom{},
                                          StackHorizontalTop{},
                                          StackHorizontalDouble{} }) {
    EXPECT_TRUE(is_stack_horizontal(p));
    EXPECT_FALSE(is_stack_vertical(p));
  }
}

TEST(TestLayoutsUtils, TestIsStackVerticalLayout) {
  using namespace ymwm::layouts;
  for (const Parameters& p : std::initializer_list<Parameters>{
           StackVerticalLeft{}, StackVerticalRight{}, StackVerticalDouble{} }) {
    EXPECT_TRUE(is_stack_vertical(p));
    EXPECT_FALSE(is_stack_horizontal(p));
  }
}

TEST(TestLayoutsUtils, TestIsStackLayout) {
  using namespace ymwm::layouts;
  for (const Parameters& p :
       std::initializer_list<Parameters>{ StackVerticalLeft{},
                                          StackVerticalRight{},
                                          StackVerticalDouble{},
                                          StackHorizontalBottom{},
                                          StackHorizontalTop{},
                                          StackHorizontalDouble{} }) {
    EXPECT_TRUE(is_stack(p));
  }
}
