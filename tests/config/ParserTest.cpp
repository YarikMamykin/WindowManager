#include "config/Parser.h"

#include "config/Layout.h"
#include "config/Window.h"
#include "environment/Command.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/Map.h"

#include <gtest/gtest.h>
#include <variant>

TEST(TestEventMap, CreateEventMapFromYamlFile) {
  ymwm::config::Parser parser{ "key-bindings.yaml" };
  ymwm::events::Map events_map;
  EXPECT_NO_THROW(events_map = parser.event_map());
  auto event1 = ymwm::events::AbstractKeyPress{
    .code = ymwm::events::AbstractKeyCode::A,
    .mask = ymwm::events::AbstractKeyMask::Ctrl |
            ymwm::events::AbstractKeyMask::Shift
  };
  auto event2 =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::B,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::Ctrl };
  ASSERT_TRUE(events_map.contains(event1));
  ASSERT_TRUE(events_map.contains(event2));
  ASSERT_TRUE(std::holds_alternative<ymwm::environment::commands::RunTerminal>(
      events_map.at(event1)));
  EXPECT_EQ(
      "/bin/sh",
      std::get<ymwm::environment::commands::RunTerminal>(events_map.at(event1))
          .path);
  EXPECT_TRUE(
      std::holds_alternative<ymwm::environment::commands::ExitRequested>(
          events_map.at(event2)));
  for (const auto& [k, v] : ymwm::events::default_event_map()) {
    ASSERT_TRUE(events_map.contains(k));
    ASSERT_EQ(v.index(), events_map.at(k).index());
  }
}

TEST(TestEventMap, CreateEventMapFromYamlFileWithoutMasksSpecified) {
  ymwm::config::Parser parser{ "key-bindings-without-masks.yaml" };
  ymwm::events::Map events_map;
  EXPECT_NO_THROW(events_map = parser.event_map());
  auto event1 =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::A,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::NONE };
  auto event2 =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::B,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::NONE };
  ASSERT_TRUE(events_map.contains(event1));
  ASSERT_TRUE(events_map.contains(event2));
  EXPECT_TRUE(std::holds_alternative<ymwm::environment::commands::RunTerminal>(
      events_map.at(event1)));
  EXPECT_TRUE(
      std::holds_alternative<ymwm::environment::commands::ExitRequested>(
          events_map.at(event2)));
}

TEST(TestEventMap, CreateEventMapFromYamlFileWithoutCmdSpecified) {
  EXPECT_THROW(
      ymwm::config::Parser parser{ "key-bindings-without-cmd-specified.yaml" },
      ymwm::config::ParsingError);
}

TEST(TestLayoutsConfig, EachLayoutConfig) {
  ymwm::config::Parser parser{ "layouts.yaml" };
  EXPECT_EQ(1, ymwm::config::layouts::screen_margins.left);
  EXPECT_EQ(2, ymwm::config::layouts::screen_margins.right);
  EXPECT_EQ(3, ymwm::config::layouts::screen_margins.top);
  EXPECT_EQ(4, ymwm::config::layouts::screen_margins.bottom);
  EXPECT_EQ(12, ymwm::config::layouts::grid::grid_margins.horizontal);
  EXPECT_EQ(34, ymwm::config::layouts::grid::grid_margins.vertical);
  EXPECT_EQ(80, ymwm::config::layouts::stack_vertical_right::main_window_ratio);
  EXPECT_EQ(12,
            ymwm::config::layouts::stack_vertical_right::main_window_margin);
  EXPECT_EQ(11,
            ymwm::config::layouts::stack_vertical_right::stack_window_margin);
}

TEST(TestLayoutsConfig, EachWindowConfig) {
  ymwm::config::Parser parser{ "windows.yaml" };
  EXPECT_EQ(10, ymwm::config::windows::focused_border_width);
  EXPECT_EQ(255, ymwm::config::windows::focused_border_color.red);
  EXPECT_EQ(128, ymwm::config::windows::focused_border_color.green);
  EXPECT_EQ(39, ymwm::config::windows::focused_border_color.blue);
  EXPECT_EQ(13, ymwm::config::windows::regular_border_width);
  EXPECT_EQ(25, ymwm::config::windows::regular_border_color.red);
  EXPECT_EQ(12, ymwm::config::windows::regular_border_color.green);
  EXPECT_EQ(38, ymwm::config::windows::regular_border_color.blue);
}

TEST(TestEventMap, RemoveDuplicatedCommadsFromParsedEventMap) {
  ymwm::config::Parser parser{ "key-bindings-with-repeated-cmds.yaml" };
  auto events_map = parser.event_map();

  auto expected_event1 = ymwm::events::AbstractKeyPress{
    .code = ymwm::events::AbstractKeyCode::A,
    .mask = ymwm::events::AbstractKeyMask::Ctrl |
            ymwm::events::AbstractKeyMask::Shift
  };

  ymwm::environment::commands::Command expected_cmd1 =
      ymwm::environment::commands::RunTerminal{ .path = { "/bin/sh" } };
  ASSERT_TRUE(events_map.contains(expected_event1));
  ASSERT_EQ(expected_cmd1, events_map.at(expected_event1));

  auto expected_event2 =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::B,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::Alt };

  ymwm::environment::commands::Command expected_cmd2 =
      ymwm::environment::commands::ExitRequested{};
  ASSERT_TRUE(events_map.contains(expected_event2));
  ASSERT_EQ(expected_cmd2, events_map.at(expected_event2));

  auto events_removed = parser.events_removed();
  ASSERT_EQ(1ul, events_removed.size());
  ymwm::events::Event expected_removed_event =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::B,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::Ctrl };

  ASSERT_EQ(expected_removed_event, events_removed.front());
}
