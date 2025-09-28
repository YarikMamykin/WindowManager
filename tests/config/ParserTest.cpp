#include "config/Parser.h"

#include "config/Layout.h"
#include "config/Misc.h"
#include "config/Window.h"
#include "environment/Command.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/AbstractMouseCode.h"
#include "events/AbstractMousePress.h"
#include "events/Map.h"

#include <algorithm>
#include <filesystem>
#include <format>
#include <gtest/gtest.h>
#include <variant>

std::string event_to_string(const ymwm::events::Event& event) {
  auto visitor = [](const auto& e) -> std::string {
    if constexpr (std::is_same_v<const ymwm::events::AbstractKeyPress&,
                                 decltype(e)>) {
      return std::format("Type: {}, Key: {}, Mask: {}", e.type, e.code, e.mask);
    } else {
      return std::format("Type: {}", e.type.data());
    }
  };
  return std::visit(visitor, event);
}

std::string
command_to_string(const ymwm::environment::commands::Command& command) {
  auto visitor = [](const auto& command) -> std::string {
    if constexpr (std::is_same_v<
                      const ymwm::environment::commands::RunTerminal&,
                      decltype(command)>) {
      return std::format("Type: {}, Args: {}", command.type, command.path);
    } else if constexpr (std::is_same_v<
                             const ymwm::environment::commands::SetLayout&,
                             decltype(command)>) {
      return std::format("Type: {}, Args: {}", command.type, command.layout);
    } else {
      return std::format("Type: {}", command.type);
    }
  };
  return std::visit(visitor, command);
}

static inline std::string
print_event_maps(const ymwm::events::Map& event_map1,
                 const ymwm::events::Map& event_map2) noexcept {
  std::string result;
  auto event_map1_it = event_map1.begin();
  auto event_map2_it = event_map2.begin();
  for (; event_map1_it != event_map1.end() and
         event_map2_it != event_map2.end();) {
    result += std::format(
        "Event1_1: {}\tEvent1_2: {}\nCommand1_1: {}\tCommand1_2: {}\n\n",
        event_to_string(event_map1_it->first),
        event_to_string(event_map2_it->first),
        command_to_string(event_map1_it->second),
        command_to_string(event_map2_it->second));
    std::advance(event_map1_it, 1);
    std::advance(event_map2_it, 1);
  }
  return result;
}

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
  EXPECT_EQ(70, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  EXPECT_EQ(13, ymwm::config::layouts::stack_horizontal::main_window_margin);
  EXPECT_EQ(10, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  EXPECT_EQ(10, ymwm::config::layouts::parallel::margin);
  EXPECT_EQ(90, ymwm::config::layouts::centered::window_width_ratio);
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

  expected_removed_event = ymwm::events::AbstractKeyPress{
    .code = ymwm::events::AbstractKeyCode::Q,
    .mask = ymwm::events::AbstractKeyMask::Alt |
            ymwm::events::AbstractKeyMask::Shift
  };
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
  EXPECT_EQ(70, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  EXPECT_EQ(13, ymwm::config::layouts::stack_horizontal::main_window_margin);
  EXPECT_EQ(10, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  EXPECT_EQ(10, ymwm::config::layouts::parallel::margin);
  EXPECT_EQ(90, ymwm::config::layouts::centered::window_width_ratio);
  EXPECT_EQ("us,ru,ua", ymwm::config::misc::language_layout);
  EXPECT_EQ("/tmp/some.png", ymwm::config::misc::background_image_path);
  EXPECT_EQ("/home/user/Pictures", ymwm::config::misc::screenshots_folder);
  EXPECT_FALSE(ymwm::config::windows::autofocus_on_mousehover);
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
  auto event3 = ymwm::events::AbstractMousePress{
    .mask = ymwm::events::AbstractKeyMask::Shift,
    .mcode = ymwm::events::AbstractMouseCode::Left
  };
  auto event4 = ymwm::events::AbstractKeyPress{
    .code = ymwm::events::AbstractKeyCode::Number_0,
    .mask = ymwm::events::AbstractKeyMask::Alt,
  };
  ASSERT_TRUE(events_map.contains(event1));
  ASSERT_TRUE(events_map.contains(event2));
  ASSERT_TRUE(events_map.contains(event3));
  ASSERT_TRUE(std::holds_alternative<ymwm::environment::commands::RunTerminal>(
      events_map.at(event1)));
  EXPECT_EQ(
      "/bin/sh",
      std::get<ymwm::environment::commands::RunTerminal>(events_map.at(event1))
          .path);
  EXPECT_TRUE(
      std::holds_alternative<ymwm::environment::commands::ExitRequested>(
          events_map.at(event2)));
  EXPECT_TRUE(
      std::holds_alternative<ymwm::environment::commands::TakeScreenshot>(
          events_map.at(event3)));
  EXPECT_TRUE(
      std::holds_alternative<ymwm::environment::commands::FocusNthWindow>(
          events_map.at(event4)));
  for (const auto& [k, v] : ymwm::events::default_event_map()) {
    if (not std::holds_alternative<ymwm::environment::commands::ExitRequested>(
            v) and
        not std::holds_alternative<ymwm::environment::commands::TakeScreenshot>(
            v) and
        not std::holds_alternative<ymwm::environment::commands::FocusNthWindow>(
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
  EXPECT_EQ("/tmp/test.log", ymwm::config::misc::logfile);
}

TEST(TestConfig, GenerateConfigToFile) {
  ymwm::config::layouts::screen_margins = { 10, 11, 21, 34 };
  ymwm::config::layouts::grid::grid_margins = { 1, 2 };
  ymwm::config::layouts::parallel::margin = 123;
  ymwm::config::layouts::centered::window_width_ratio = 74;
  ymwm::config::layouts::stack_vertical::main_window_margin = 51;
  ymwm::config::layouts::stack_vertical::stack_window_margin = 52;
  ymwm::config::layouts::stack_vertical::main_window_ratio = 53;
  ymwm::config::layouts::stack_horizontal::main_window_margin = 48;
  ymwm::config::layouts::stack_horizontal::stack_window_margin = 49;
  ymwm::config::layouts::stack_horizontal::main_window_ratio = 73;
  ymwm::config::windows::regular_border_color = { 0xff, 0x10, 0xfe };
  ymwm::config::windows::focused_border_color = { 0xaa, 0xdc, 0xee };
  ymwm::config::windows::regular_border_width = 1;
  ymwm::config::windows::focused_border_width = 2;
  ymwm::config::misc::language_layout = "ru,fr,de";
  ymwm::config::misc::background_image_path = "/tmp/someimg.png";
  ymwm::config::misc::screenshots_folder = "/home/user/folder";
  ymwm::config::windows::autofocus_on_mousehover = false;

  const std::filesystem::path test_config_path =
      std::filesystem::temp_directory_path() / "test-config.yaml";
  EXPECT_NO_THROW(
      ymwm::config::Parser::default_config_to_yaml(test_config_path.string()));
  ASSERT_TRUE(std::filesystem::exists(test_config_path));

  ymwm::config::Parser parser{ test_config_path.string() };
  auto parsed_event_map = parser.event_map();
  const auto default_event_map = ymwm::events::default_event_map();

  EXPECT_EQ(10, ymwm::config::layouts::screen_margins.left);
  EXPECT_EQ(11, ymwm::config::layouts::screen_margins.right);
  EXPECT_EQ(21, ymwm::config::layouts::screen_margins.top);
  EXPECT_EQ(34, ymwm::config::layouts::screen_margins.bottom);
  EXPECT_EQ(1, ymwm::config::layouts::grid::grid_margins.horizontal);
  EXPECT_EQ(2, ymwm::config::layouts::grid::grid_margins.vertical);
  EXPECT_EQ(123, ymwm::config::layouts::parallel::margin);
  EXPECT_EQ(74, ymwm::config::layouts::centered::window_width_ratio);
  EXPECT_EQ(53, ymwm::config::layouts::stack_vertical::main_window_ratio);
  EXPECT_EQ(51, ymwm::config::layouts::stack_vertical::main_window_margin);
  EXPECT_EQ(52, ymwm::config::layouts::stack_vertical::stack_window_margin);
  EXPECT_EQ(73, ymwm::config::layouts::stack_horizontal::main_window_ratio);
  EXPECT_EQ(48, ymwm::config::layouts::stack_horizontal::main_window_margin);
  EXPECT_EQ(49, ymwm::config::layouts::stack_horizontal::stack_window_margin);
  EXPECT_EQ(1, ymwm::config::windows::regular_border_width);
  EXPECT_EQ(2, ymwm::config::windows::focused_border_width);
  EXPECT_EQ(0xff, ymwm::config::windows::regular_border_color.red);
  EXPECT_EQ(0x10, ymwm::config::windows::regular_border_color.green);
  EXPECT_EQ(0xfe, ymwm::config::windows::regular_border_color.blue);
  EXPECT_EQ(0xaa, ymwm::config::windows::focused_border_color.red);
  EXPECT_EQ(0xdc, ymwm::config::windows::focused_border_color.green);
  EXPECT_EQ(0xee, ymwm::config::windows::focused_border_color.blue);
  EXPECT_EQ("ru,fr,de", ymwm::config::misc::language_layout);
  EXPECT_EQ("/tmp/someimg.png", ymwm::config::misc::background_image_path);
  EXPECT_EQ("/home/user/folder", ymwm::config::misc::screenshots_folder);
  EXPECT_FALSE(ymwm::config::windows::autofocus_on_mousehover);

  ASSERT_EQ(parsed_event_map.size(), default_event_map.size());

  for (const auto& [event, command] : default_event_map) {
    EXPECT_TRUE(parsed_event_map.contains(event)) << event_to_string(event);
  }

  EXPECT_EQ(parsed_event_map, default_event_map)
      << print_event_maps(parsed_event_map, default_event_map);
}
