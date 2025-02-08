#include "config/Parser.h"

#include "config/Layout.h"
#include "config/Misc.h"
#include "config/Window.h"
#include "environment/Command.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/Map.h"

#include <algorithm>
#include <gtest/gtest.h>
#include <variant>

TEST(TestEventMap, CreateEventMapFromYamlFile) {
  ymwm::config::Parser parser{ "key-bindings.yaml" };
  ymwm::events::Map events_map;
  EXPECT_NO_THROW(events_map = parser.event_map());
  ymwm::events::Event event1 = ymwm::events::AbstractKeyPress{
    .code = ymwm::events::AbstractKeyCode::A,
    .mask = ymwm::events::AbstractKeyMask::Ctrl |
            ymwm::events::AbstractKeyMask::Shift
  };
  ymwm::events::Event event2 =
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
    if (not std::holds_alternative<ymwm::environment::commands::ExitRequested>(
            v)) {
      ASSERT_TRUE(events_map.contains(k));
      ASSERT_EQ(v.index(), events_map.at(k).index());
    }
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
  EXPECT_EQ(80, ymwm::config::layouts::stack_vertical::main_window_ratio);
  EXPECT_EQ(12, ymwm::config::layouts::stack_vertical::main_window_margin);
  EXPECT_EQ(11, ymwm::config::layouts::stack_vertical::stack_window_margin);
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

  auto expected_event1 =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::B,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::Ctrl };

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
  ASSERT_EQ(2ul, events_removed.size());
  ymwm::events::Event expected_removed_event = ymwm::events::AbstractKeyPress{
    .code = ymwm::events::AbstractKeyCode::A,
    .mask = ymwm::events::AbstractKeyMask::Ctrl |
            ymwm::events::AbstractKeyMask::Shift
  };
  ASSERT_EQ(expected_removed_event, events_removed.front());

  expected_removed_event =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::E,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::Ctrl };
  ASSERT_EQ(expected_removed_event, events_removed.back());
}

TEST(TestConfig, AllValidConfig) {
  ymwm::config::Parser parser{ "all-config.yaml" };
  EXPECT_EQ(10, ymwm::config::windows::focused_border_width);
  EXPECT_EQ(255, ymwm::config::windows::focused_border_color.red);
  EXPECT_EQ(128, ymwm::config::windows::focused_border_color.green);
  EXPECT_EQ(39, ymwm::config::windows::focused_border_color.blue);
  EXPECT_EQ(13, ymwm::config::windows::regular_border_width);
  EXPECT_EQ(25, ymwm::config::windows::regular_border_color.red);
  EXPECT_EQ(12, ymwm::config::windows::regular_border_color.green);
  EXPECT_EQ(38, ymwm::config::windows::regular_border_color.blue);
  EXPECT_EQ(1, ymwm::config::layouts::screen_margins.left);
  EXPECT_EQ(2, ymwm::config::layouts::screen_margins.right);
  EXPECT_EQ(3, ymwm::config::layouts::screen_margins.top);
  EXPECT_EQ(4, ymwm::config::layouts::screen_margins.bottom);
  EXPECT_EQ(12, ymwm::config::layouts::grid::grid_margins.horizontal);
  EXPECT_EQ(34, ymwm::config::layouts::grid::grid_margins.vertical);
  EXPECT_EQ(80, ymwm::config::layouts::stack_vertical::main_window_ratio);
  EXPECT_EQ(12, ymwm::config::layouts::stack_vertical::main_window_margin);
  EXPECT_EQ(11, ymwm::config::layouts::stack_vertical::stack_window_margin);
  EXPECT_EQ("us,ru,ua", ymwm::config::misc::language_layout);
  EXPECT_EQ("/tmp/some.png", ymwm::config::misc::background_image_path);
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
    if (not std::holds_alternative<ymwm::environment::commands::ExitRequested>(
            v)) {
      ASSERT_TRUE(events_map.contains(k));
      ASSERT_EQ(v.index(), events_map.at(k).index());
    }
  }
}

TEST(TestConfig, OverrideDefaultKeyBinding) {
  ymwm::config::Parser parser{ "key-bindings-default-overriden.yaml" };
  auto event_map = parser.event_map();

  auto found_event_to_cmd = std::find_if(
      event_map.begin(), event_map.end(), [](const auto& event_to_cmd) -> bool {
        return std::holds_alternative<
            ymwm::environment::commands::ExitRequested>(event_to_cmd.second);
      });
  ASSERT_NE(event_map.end(), found_event_to_cmd);

  ymwm::events::Event exit_requested_event = ymwm::events::AbstractKeyPress{
    .code = ymwm::events::AbstractKeyCode::L,
    .mask = ymwm::events::AbstractKeyMask::Alt |
            ymwm::events::AbstractKeyMask::Shift
  };
  ASSERT_EQ(exit_requested_event, found_event_to_cmd->first);

  found_event_to_cmd = std::find_if(
      event_map.begin(), event_map.end(), [](const auto& event_to_cmd) -> bool {
        return std::holds_alternative<ymwm::environment::commands::CloseWindow>(
            event_to_cmd.second);
      });
  ASSERT_NE(event_map.end(), found_event_to_cmd);

  ymwm::events::Event close_window_event =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::E,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::Ctrl };
  ASSERT_EQ(close_window_event, found_event_to_cmd->first);
}

TEST(TestConfig, ParseMisc) {
  ymwm::config::Parser parser{ "misc.yaml" };
  EXPECT_EQ("us,ru,ua", ymwm::config::misc::language_layout);
  EXPECT_EQ("/tmp/some.png", ymwm::config::misc::background_image_path);
}
