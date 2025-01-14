#include "TestEnvironment.h"
#include "common/Color.h"
#include "config/Layout.h"
#include "config/Window.h"
#include "layouts/Layout.h"
#include "layouts/Parameters.h"
#include "window/Manager.h"
#include "window/Window.h"

#include "gmock/gmock.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

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
      .Times(2)
      .WillRepeatedly(testing::SaveArg<0>(windows_in_params.data() + 1ul));
  EXPECT_CALL(tenv, focus_window)
      .WillOnce(testing::SaveArg<0>(windows_in_params.data() + 2ul));
  m.add_window(ymwm::window::Window{ .id = 1 });

  for (const auto& w : windows_in_params) {
    EXPECT_EQ(1, w.id);
  }

  std::array<ymwm::window::Window, 4ul> second_windows_in_params;
  auto second_windows_in_params_it = second_windows_in_params.begin();
  auto save_window =
      [&second_windows_in_params_it](const ymwm::window::Window& w) -> void {
    *second_windows_in_params_it = w;
    std::advance(second_windows_in_params_it, 1);
  };
  EXPECT_CALL(tenv, screen_width_and_height).Times(1);
  EXPECT_CALL(tenv, move_and_resize).WillRepeatedly(save_window);
  EXPECT_CALL(tenv, update_window_border).Times(2).WillRepeatedly(save_window);
  EXPECT_CALL(tenv, focus_window).WillOnce(save_window);
  m.add_window(ymwm::window::Window{ .id = 2 });

  const std::array<ymwm::environment::ID, second_windows_in_params.size()>
      expected_ids{ 1, 2, 2, 1 };
  for (auto i = 0ul; i < second_windows_in_params.size(); ++i) {
    EXPECT_EQ(second_windows_in_params.at(i).id, expected_ids.at(i));
  }
}

TEST(TestWindowManager, AddRemoveWindows_LayoutAppliedAfterEachAction) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };

  m.layout().update(ymwm::layouts::Maximised{});

  EXPECT_CALL(tenv, screen_width_and_height);
  EXPECT_CALL(tenv, move_and_resize);
  EXPECT_CALL(tenv, update_window_border).Times(2);
  EXPECT_CALL(tenv, focus_window);
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
  EXPECT_CALL(tenv, update_window_border).Times(4);
  EXPECT_CALL(tenv, focus_window);
  m.add_window(ymwm::window::Window{ .id = 2 });

  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 1,
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 2,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(m.focus().window()->get().id, 2);

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
  m.layout().update(ymwm::layouts::Maximised{});

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
  m.layout().update(ymwm::layouts::Maximised{});

  EXPECT_CALL(tenv, update_window_border).Times(2);
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
  m.layout().update(ymwm::layouts::Maximised{});

  EXPECT_CALL(tenv, update_window_border).Times(2);
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
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Maximised{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(2, m.focus().window()->get().id);

  EXPECT_CALL(tenv, screen_width_and_height);
  EXPECT_CALL(tenv, focus_window);
  m.move_focused_window_forward();
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(2, m.focus().window()->get().id);
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color },
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color }));
}

TEST(TestWindowManager, MoveFocusedWindowBackward) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Maximised{});

  m.add_window(ymwm::window::Window{ .id = 1 });
  m.add_window(ymwm::window::Window{ .id = 2 });
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color }));
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(2, m.focus().window()->get().id);

  EXPECT_CALL(tenv, screen_width_and_height);
  EXPECT_CALL(tenv, focus_window);
  m.move_focused_window_backward();
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(2, m.focus().window()->get().id);
  EXPECT_THAT(
      m.windows(),
      testing::ElementsAre(
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color },
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color }));
}

TEST(TestWindowManager, AddWindow_LastWindowIsFocusedEachTime) {
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
  EXPECT_EQ(2, m.focus().window()->get().id);

  m.add_window(ymwm::window::Window{ .id = 3 });
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(3, m.focus().window()->get().id);

  m.focus().prev_window();
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(2, m.focus().window()->get().id);

  m.add_window(ymwm::window::Window{ .id = 4 });
  ASSERT_TRUE(m.focus().window());
  EXPECT_EQ(4, m.focus().window()->get().id);
}

TEST(TestWindowManager, SwapFocusedWindowWithTopOne) {
  ymwm::environment::TestEnvironment tenv;
  ON_CALL(tenv, screen_width_and_height)
      .WillByDefault(testing::Return(std::make_tuple(1000, 1000)));

  ymwm::window::Manager m{ &tenv };
  m.layout().update(ymwm::layouts::Maximised{});

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
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 2,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 3,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .h = 1000 - (2 * ymwm::config::windows::focused_border_width),
              .border_width = ymwm::config::windows::focused_border_width,
              .border_color = focused_color }));
  ASSERT_TRUE(m.focus().window());
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
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color },
          ymwm::window::Window{
              .id = 1,
              .x = 0,
              .y = 0,
              .w = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .h = 1000 - (2 * ymwm::config::windows::regular_border_width),
              .border_width = ymwm::config::windows::regular_border_width,
              .border_color = regular_color }));
}
