#include "TestEnvironment.h"
#include "common/Color.h"
#include "common/Direction.h"
#include "config/Layout.h"
#include "config/Window.h"
#include "layouts/Grid.h"
#include "layouts/Layout.h"
#include "layouts/Parameters.h"
#include "layouts/StackHorizontalBottom.h"
#include "layouts/StackVerticalDouble.h"
#include "window/GroupManager.h"
#include "window/Manager.h"
#include "window/Window.h"

#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <variant>

static const ymwm::common::Color regular_color =
    ymwm::config::windows::regular_border_color;
static const ymwm::common::Color focused_color =
    ymwm::config::windows::focused_border_color;

static inline std::string
window_to_string(const ymwm::window::Window& w) noexcept {
  return std::format("id: {}; x: {}; y: {}; w: {}; h: {}; bw: {}; bc: {},{},{}",
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

TEST(TestWindowManager, SetLayoutNoWindows_NoWindowResizeCalled) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };

  EXPECT_CALL(tenv, screen_width_and_height).Times(0);
  EXPECT_CALL(tenv, move_and_resize).Times(0);
  m.layout().update();
}

TEST(TestWindowManager, SetLayoutSeveralWindows_WindowResizeCalled) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };

  EXPECT_CALL(tenv, screen_width_and_height).Times(0);
  EXPECT_CALL(tenv, move_and_resize).Times(0);
  m.layout().update();

  std::array<ymwm::window::Window, 3ul> windows_in_params;

  EXPECT_CALL(tenv, screen_width_and_height).Times(1);
  EXPECT_CALL(tenv, move_and_resize)
      .WillOnce(testing::SaveArg<0>(windows_in_params.data()));
  EXPECT_CALL(tenv, update_window_border)
      .Times(4)
      .WillRepeatedly(testing::SaveArg<0>(windows_in_params.data() + 1ul));
  EXPECT_CALL(tenv, focus_window)
      .WillRepeatedly(testing::SaveArg<0>(windows_in_params.data() + 2ul));
  m.add_window(ymwm::window::Window{ .id = 1 });

  for (const auto& w : windows_in_params) {
    EXPECT_EQ(1, w.id);
  }

  EXPECT_CALL(tenv, screen_width_and_height).Times(1);
  EXPECT_CALL(tenv, move_and_resize).Times(2);
  EXPECT_CALL(tenv, update_window_border).Times(4);
  EXPECT_CALL(tenv, focus_window).Times(2);
  m.add_window(ymwm::window::Window{ .id = 2 });
}

TEST(TestWindowManager, AddRemoveWindows_LayoutAppliedAfterEachAction) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };

  m.layout().update(ymwm::layouts::Centered{});

  EXPECT_CALL(tenv, screen_width_and_height);
  EXPECT_CALL(tenv, move_and_resize);
  EXPECT_CALL(tenv, update_window_border).Times(4);
  EXPECT_CALL(tenv, focus_window).Times(2);
  m.add_window(ymwm::window::Window{ .id = 1 });
  EXPECT_THAT(m.windows(),
              testing::ElementsAre(ymwm::window::Window{
                  .id = 1,
                  .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .border_width = ymwm::config::windows::focused_border_width,
                  .border_color = focused_color }))
      << window_to_string(m.windows().front());
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(m.focus().window()->get().id, 1);

  EXPECT_CALL(tenv, screen_width_and_height);
  EXPECT_CALL(tenv, move_and_resize).Times(2);
  EXPECT_CALL(tenv, update_window_border).Times(6);
  EXPECT_CALL(tenv, focus_window).Times(2);
  m.add_window(ymwm::window::Window{ .id = 2 });

  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 1,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color },
          ymwm::window::Window{
              .id = 2,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(m.focus().window()->get().id, 1);

  EXPECT_CALL(tenv, screen_width_and_height);
  EXPECT_CALL(tenv, move_and_resize);
  EXPECT_CALL(tenv, focus_window);
  m.remove_window(2);
  EXPECT_THAT(m.windows(),
              testing::ElementsAre(ymwm::window::Window{
                  .id = 1,
                  .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .border_width = ymwm::config::windows::focused_border_width,
                  .border_color = focused_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(m.focus().window()->get().id, 1);

  EXPECT_CALL(tenv, screen_width_and_height).Times(0);
  EXPECT_CALL(tenv, move_and_resize).Times(0);
  EXPECT_CALL(tenv, focus_window).Times(0);
  m.remove_window(1);
  EXPECT_FALSE(m.focus().window());
}

TEST(TestWindowManager, MoveFocusedWindowToCoords) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Centered{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  EXPECT_THAT(m.windows(),
              testing::ElementsAre(ymwm::window::Window{
                  .id = 1,
                  .x = 0,
                  .y = 0,
                  .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .border_width = ymwm::config::windows::focused_border_width,
                  .border_color = focused_color }));

  ymwm::window::Window passed_window;
  EXPECT_CALL(tenv, move_and_resize)
      .WillOnce(testing::SaveArg<0>(&passed_window));
  m.move_focused_window_to(100, 100);
  EXPECT_EQ(passed_window.id, 1);
  EXPECT_EQ(passed_window.x, 100);
  EXPECT_EQ(passed_window.y, 100);
  EXPECT_EQ(passed_window.w,
            1000 - (2 * ymwm::config::windows::focused_border_width));
  EXPECT_EQ(passed_window.h,
            1000 - (2 * ymwm::config::windows::focused_border_width));
  EXPECT_THAT(m.windows(),
              testing::ElementsAre(ymwm::window::Window{
                  .id = 1,
                  .x = 100,
                  .y = 100,
                  .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .border_width = ymwm::config::windows::focused_border_width,
                  .border_color = focused_color }));
}

TEST(TestWindowManager, ResetFocusIfWindowsAreRemoved) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Centered{});

  EXPECT_CALL(tenv, update_window_border).Times(4);
  EXPECT_CALL(tenv, move_and_resize);
  m.add_window(ymwm::window::Window{ .id = 1 });
  EXPECT_THAT(m.windows(),
              testing::ElementsAre(ymwm::window::Window{
                  .id = 1,
                  .x = 0,
                  .y = 0,
                  .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .border_width = ymwm::config::windows::focused_border_width,
                  .border_color = focused_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);

  EXPECT_CALL(tenv, update_window_border).Times(0);
  EXPECT_CALL(tenv, move_and_resize).Times(0);
  EXPECT_CALL(tenv, reset_focus).Times(1);
  m.remove_window(1);
  ASSERT_FALSE(m.focus().window());
  ASSERT_TRUE(m.windows().empty());
}

TEST(TestWindowManager, CloseFocusedWindow) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Centered{});

  EXPECT_CALL(tenv, update_window_border).Times(4);
  EXPECT_CALL(tenv, move_and_resize);
  m.add_window(ymwm::window::Window{ .id = 1 });
  EXPECT_THAT(m.windows(),
              testing::ElementsAre(ymwm::window::Window{
                  .id = 1,
                  .x = 0,
                  .y = 0,
                  .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .border_width = ymwm::config::windows::focused_border_width,
                  .border_color = focused_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);

  ymwm::window::Window passed_window;
  EXPECT_CALL(tenv, close_window).WillOnce(testing::SaveArg<0>(&passed_window));
  m.close_focused_window();

  // Expect close_window call for last window in Manager,
  // as Manager sends `close` to windows left in Manager.
  EXPECT_CALL(tenv, close_window);

  ASSERT_TRUE(m.focus().window());
  ASSERT_THAT(m.windows(),
              testing::ElementsAre(ymwm::window::Window{
                  .id = 1,
                  .x = 0,
                  .y = 0,
                  .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
                  .border_width = ymwm::config::windows::focused_border_width,
                  .border_color = focused_color }));
  EXPECT_EQ(passed_window, m.focus().window()->get());
}

TEST(TestWindowManager, MoveFocusedWindowForward) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Centered{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color },
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);

  EXPECT_CALL(tenv, screen_width_and_height);
  EXPECT_CALL(tenv, focus_window);
  m.move_focused_window_forward();
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color }));
}

TEST(TestWindowManager, MoveFocusedWindowBackward) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Centered{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color },
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);

  EXPECT_CALL(tenv, screen_width_and_height);
  EXPECT_CALL(tenv, focus_window);
  m.move_focused_window_backward();
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color }));
}

TEST(TestWindowManager, AddWindow_CurrentWindowIsFocusedEachTime) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };

  ASSERT_FALSE(m.focus().window());

  m.add_window(ymwm::window::Window{ .id = 1 });
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);

  m.add_window(ymwm::window::Window{ .id = 2 });
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);

  m.add_window(ymwm::window::Window{ .id = 3 });
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(1, m.focus().window()->get().id);

  m.focus().prev_window();
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(3, m.focus().window()->get().id);

  m.add_window(ymwm::window::Window{ .id = 4 });
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(3, m.focus().window()->get().id);
}

TEST(TestWindowManager, SwapFocusedWindowWithTopOne) {
  ymwm::config::windows::regular_border_width = 0;
  ymwm::config::windows::focused_border_width = 2;

  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Centered{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color },
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 3,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color }));
  ASSERT_TRUE(m.focus().window());
  ASSERT_EQ(1, m.focus().window()->get().id);
  m.focus().last_window();
  ASSERT_EQ(3, m.focus().window()->get().id);

  EXPECT_CALL(tenv, move_and_resize).Times(3);
  EXPECT_CALL(tenv, update_window_border).Times(2);
  m.swap_focused_window_with_top();
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 3,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color },
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color }));
}

TEST(TestWindowManager, TestFocusWindowById) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Centered{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });

  ASSERT_EQ(3ul, m.windows().size());
  ASSERT_EQ(1, m.focus().window()->get().id);

  EXPECT_CALL(tenv, update_window_border).Times(2);
  EXPECT_CALL(tenv, focus_window).Times(1);

  m.focus().window(2);
  ASSERT_EQ(2, m.focus().window()->get().id);

  EXPECT_CALL(tenv, update_window_border).Times(2);
  EXPECT_CALL(tenv, focus_window).Times(1);

  m.focus().window(3);
  ASSERT_EQ(3, m.focus().window()->get().id);
}

TEST(FocusManager, MoveFocusOnGrid) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Grid{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });
  m.add_window(ymwm::window::Window{ .id = 4 });
  m.add_window(ymwm::window::Window{ .id = 5 });

  m.focus().last_window();

  ASSERT_EQ(5, m.focus().window()->get().id);
  ASSERT_EQ(5ul, m.windows().size());
  auto params = m.layout().parameters();
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::Grid>(params));
  auto grid_params = std::get<ymwm::layouts::Grid>(params);
  ASSERT_EQ(3ul, grid_params.grid_size);

  m.focus().move(ymwm::common::Direction::Left, grid_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  // Verify repeated left move on left-most column has no effect.
  m.focus().move(ymwm::common::Direction::Left, grid_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, grid_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  // Verify repeated right move on right-most column has no effect.
  m.focus().move(ymwm::common::Direction::Right, grid_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, grid_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  // Verify repeated up move on top-most column has no effect.
  m.focus().move(ymwm::common::Direction::Up, grid_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, grid_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  // Verify down move has no effect, if grid is not filled completely.
  m.focus().move(ymwm::common::Direction::Down, grid_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, grid_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, grid_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  // Verify repeated down move has no effect.
  m.focus().move(ymwm::common::Direction::Down, grid_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, grid_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  // Able to go up in left-most column.
  m.focus().move(ymwm::common::Direction::Up, grid_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
}

TEST(FocusManager, MoveFocusOnGrid_OneWindowInGridOnly) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Grid{});

  m.add_window(ymwm::window::Window{ .id = 1 });

  ASSERT_EQ(1, m.focus().window()->get().id);
  ASSERT_EQ(1ul, m.windows().size());
  auto params = m.layout().parameters();
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::Grid>(params));
  auto grid_params = std::get<ymwm::layouts::Grid>(params);
  ASSERT_EQ(2ul, grid_params.grid_size);

  m.focus().move(ymwm::common::Direction::Left, grid_params, 1ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
  m.focus().move(ymwm::common::Direction::Right, grid_params, 1ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
  m.focus().move(ymwm::common::Direction::Up, grid_params, 1ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
  m.focus().move(ymwm::common::Direction::Down, grid_params, 1ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
}

TEST(TestLayoutManager, TestRotateStackLayout) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::StackVerticalRight{});

  auto params = m.layout().parameters();
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::StackVerticalRight>(params));

  for (auto contains_expected_parameters : std::initializer_list<
           std::function<bool(const ymwm::layouts::Parameters&)>>{
           [](const ymwm::layouts::Parameters& p) -> bool {
             return std::holds_alternative<ymwm::layouts::StackHorizontalTop>(
                 p);
           },
           [](const ymwm::layouts::Parameters& p) -> bool {
             return std::holds_alternative<ymwm::layouts::StackVerticalLeft>(p);
           },
           [](const ymwm::layouts::Parameters& p) -> bool {
             return std::holds_alternative<
                 ymwm::layouts::StackHorizontalBottom>(p);
           },
           [](const ymwm::layouts::Parameters& p) -> bool {
             return std::holds_alternative<ymwm::layouts::StackVerticalRight>(
                 p);
           } }) {
    m.layout().rotate();
    params = m.layout().parameters();
    ASSERT_TRUE(contains_expected_parameters(params));
  }

  m.layout().update(ymwm::layouts::StackVerticalDouble{});
  params = m.layout().parameters();
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::StackVerticalDouble>(params));

  for (auto contains_expected_parameters : std::initializer_list<
           std::function<bool(const ymwm::layouts::Parameters&)>>{
           [](const ymwm::layouts::Parameters& p) -> bool {
             return std::holds_alternative<
                 ymwm::layouts::StackHorizontalDouble>(p);
           },
           [](const ymwm::layouts::Parameters& p) -> bool {
             return std::holds_alternative<ymwm::layouts::StackVerticalDouble>(
                 p);
           } }) {
    m.layout().rotate();
    params = m.layout().parameters();
    ASSERT_TRUE(contains_expected_parameters(params));
  }
}

TEST(TestFocusManager, DontFocusSameWindowTwice) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };

  m.add_window(ymwm::window::Window{ .id = 1 });

  ASSERT_EQ(1, m.focus().window()->get().id);
  ASSERT_EQ(1ul, m.windows().size());
  ASSERT_TRUE(m.focus().window());
  ASSERT_EQ(1, m.focus().window()->get().id);

  EXPECT_CALL(tenv, focus_window).Times(0);
  m.focus().window(1);
}

TEST(TestFocusManager, DontFocusNonExistingWindow) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };

  ASSERT_TRUE(m.windows().empty());
  ASSERT_FALSE(m.focus().window().has_value());

  EXPECT_CALL(tenv, focus_window).Times(0);
  m.focus().window(1);
}

TEST(TestGroupManager, TryMovingToNextGroupWhenOnlyOneExists) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::GroupManager m{ &tenv };

  m.manager().add_window({ .id = 1 });

  EXPECT_CALL(tenv, focus_window).Times(0);
  EXPECT_CALL(tenv, move_and_resize).Times(0);
  m.next();
}

TEST(TestGroupManager, TryMovingToPrevGroupWhenOnlyOneExists) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::GroupManager m{ &tenv };

  m.manager().add_window({ .id = 1 });

  EXPECT_CALL(tenv, focus_window).Times(0);
  EXPECT_CALL(tenv, move_and_resize).Times(0);
  m.prev();
}

TEST(TestGroupManager, AddGroup) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::GroupManager m{ &tenv };

  m.manager().add_window({ .id = 1 });
  ASSERT_EQ(1ul, m.manager().windows().size());
  m.add();
  ASSERT_EQ(0ul, m.manager().windows().size());

  EXPECT_CALL(tenv, move_and_resize).Times(1);
  m.next();
  ASSERT_EQ(1ul, m.manager().windows().size());
  ASSERT_EQ(1, m.manager().windows().front().id);

  // Expect hiding window of current group,
  // before moving to previous group.
  EXPECT_CALL(tenv, move_and_resize).Times(1);
  m.prev();
  ASSERT_EQ(0ul, m.manager().windows().size());
}

TEST(FocusManager, MoveFocusOnStackVerticalDouble) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::StackVerticalDouble{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });
  m.add_window(ymwm::window::Window{ .id = 4 });
  m.add_window(ymwm::window::Window{ .id = 5 });

  ASSERT_EQ(1, m.focus().window()->get().id);
  ASSERT_EQ(5ul, m.windows().size());
  auto params = m.layout().parameters();
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::StackVerticalDouble>(params));
  auto stack_params = std::get<ymwm::layouts::StackVerticalDouble>(params);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);
}

TEST(FocusManager, MoveFocusOnStackHorizontalDouble) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::StackHorizontalDouble{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });
  m.add_window(ymwm::window::Window{ .id = 4 });
  m.add_window(ymwm::window::Window{ .id = 5 });

  ASSERT_EQ(1, m.focus().window()->get().id);
  ASSERT_EQ(5ul, m.windows().size());
  auto params = m.layout().parameters();
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::StackHorizontalDouble>(params));
  auto stack_params = std::get<ymwm::layouts::StackHorizontalDouble>(params);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);
}

TEST(FocusManager, MoveFocusOnStackHorizontalTop) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::StackHorizontalTop{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });
  m.add_window(ymwm::window::Window{ .id = 4 });
  m.add_window(ymwm::window::Window{ .id = 5 });

  ASSERT_EQ(1, m.focus().window()->get().id);
  ASSERT_EQ(5ul, m.windows().size());
  auto params = m.layout().parameters();
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::StackHorizontalTop>(params));
  auto stack_params = std::get<ymwm::layouts::StackHorizontalTop>(params);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
}

TEST(FocusManager, MoveFocusOnStackHorizontalBottom) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::StackHorizontalBottom{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });
  m.add_window(ymwm::window::Window{ .id = 4 });
  m.add_window(ymwm::window::Window{ .id = 5 });

  ASSERT_EQ(1, m.focus().window()->get().id);
  ASSERT_EQ(5ul, m.windows().size());
  auto params = m.layout().parameters();
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::StackHorizontalBottom>(params));
  auto stack_params = std::get<ymwm::layouts::StackHorizontalBottom>(params);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
}

TEST(FocusManager, MoveFocusOnStackVerticalLeft) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::StackVerticalLeft{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });
  m.add_window(ymwm::window::Window{ .id = 4 });
  m.add_window(ymwm::window::Window{ .id = 5 });

  ASSERT_EQ(1, m.focus().window()->get().id);
  ASSERT_EQ(5ul, m.windows().size());
  auto params = m.layout().parameters();
  ASSERT_TRUE(std::holds_alternative<ymwm::layouts::StackVerticalLeft>(params));
  auto stack_params = std::get<ymwm::layouts::StackVerticalLeft>(params);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
}

TEST(FocusManager, MoveFocusOnStackVerticalRight) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::StackVerticalRight{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  m.add_window(ymwm::window::Window{ .id = 3 });
  m.add_window(ymwm::window::Window{ .id = 4 });
  m.add_window(ymwm::window::Window{ .id = 5 });

  ASSERT_EQ(1, m.focus().window()->get().id);
  ASSERT_EQ(5ul, m.windows().size());
  auto params = m.layout().parameters();
  ASSERT_TRUE(
      std::holds_alternative<ymwm::layouts::StackVerticalRight>(params));
  auto stack_params = std::get<ymwm::layouts::StackVerticalRight>(params);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Right, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Down, stack_params, 5ul);
  ASSERT_EQ(5, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(4, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(3, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Up, stack_params, 5ul);
  ASSERT_EQ(2, m.focus().window()->get().id);

  m.focus().move(ymwm::common::Direction::Left, stack_params, 5ul);
  ASSERT_EQ(1, m.focus().window()->get().id);
}
