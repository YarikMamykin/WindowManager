#include "environment/Command.h"
#include "events/AbstractKeyCode.h"
#include "events/AbstractKeyMask.h"
#include "events/AbstractKeyPress.h"
#include "events/Event.h"
#include "events/Map.h"

#include <filesystem>
#include <gtest/gtest.h>
#include <utility>
#include <variant>

TEST(TestEventMap,
     AddCommandUnderSameEventTwice_HaveOnlyOneRecordInMapButReplacedCommand) {
  ymwm::events::Event event{ ymwm::events::AbstractKeyPress() };
  ymwm::environment::commands::Command cmd{
    ymwm::environment::commands::RunTerminal()
  };

  ymwm::events::Map emap{
    { event, cmd }
  };

  ASSERT_EQ(1ul, emap.size());
  ASSERT_TRUE(emap.contains(event));
  ASSERT_TRUE(std::holds_alternative<ymwm::environment::commands::RunTerminal>(
      emap.at(event)));

  auto [_, inserted] = emap.insert(
      std::make_pair(event, ymwm::environment::commands::ExitRequested()));
  EXPECT_FALSE(inserted);
  EXPECT_EQ(1ul, emap.size());
  EXPECT_TRUE(emap.contains(event));
  EXPECT_FALSE(
      std::holds_alternative<ymwm::environment::commands::ExitRequested>(
          emap.at(event)));
  EXPECT_TRUE(std::holds_alternative<ymwm::environment::commands::RunTerminal>(
      emap.at(event)));
}

TEST(TestEventMap, CheckSameEventTypeDifferentByContent) {
  ymwm::environment::commands::Command cmd{
    ymwm::environment::commands::RunTerminal()
  };
  ymwm::events::Event event1{
    ymwm::events::AbstractKeyPress{ .code = 1, .mask = 0 }
  };
  ymwm::events::Event event2{
    ymwm::events::AbstractKeyPress{ .code = 2, .mask = 1 }
  };

  ymwm::events::Map emap{
    { event1, cmd },
    { event2, cmd }
  };

  EXPECT_EQ(2ul, emap.size());
  EXPECT_TRUE(std::holds_alternative<ymwm::events::AbstractKeyPress>(
      emap.begin()->first));
  EXPECT_TRUE(std::holds_alternative<ymwm::events::AbstractKeyPress>(
      std::next(emap.begin())->first));
}

TEST(TestEventMap, CreateEventMapFromYamlFile) {
  auto events_map = ymwm::events::event_map_from_yaml("key-bindings.yaml");
  ASSERT_FALSE(std::holds_alternative<std::string>(events_map))
      << std::get<std::string>(events_map);
  const auto& actual_events_map = std::get<ymwm::events::Map>(events_map);
  ASSERT_EQ(2ul, actual_events_map.size());
  auto event1 = ymwm::events::AbstractKeyPress{
    .code = ymwm::events::AbstractKeyCode::A,
    .mask = ymwm::events::AbstractKeyMask::Ctrl |
            ymwm::events::AbstractKeyMask::Shift
  };
  auto event2 =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::B,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::Ctrl };
  ASSERT_TRUE(actual_events_map.contains(event1));
  ASSERT_TRUE(actual_events_map.contains(event2));
  EXPECT_TRUE(std::holds_alternative<ymwm::environment::commands::RunTerminal>(
      actual_events_map.at(event1)));
  EXPECT_TRUE(
      std::holds_alternative<ymwm::environment::commands::ExitRequested>(
          actual_events_map.at(event2)));
}

TEST(TestEventMap, CreateEventMapFromYamlFileWithoutMasksSpecified) {
  auto events_map =
      ymwm::events::event_map_from_yaml("key-bindings-without-masks.yaml");
  ASSERT_FALSE(std::holds_alternative<std::string>(events_map))
      << std::get<std::string>(events_map);
  const auto& actual_events_map = std::get<ymwm::events::Map>(events_map);
  ASSERT_EQ(2ul, actual_events_map.size());
  auto event1 =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::A,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::NONE };
  auto event2 =
      ymwm::events::AbstractKeyPress{ .code = ymwm::events::AbstractKeyCode::B,
                                      .mask =
                                          ymwm::events::AbstractKeyMask::NONE };
  ASSERT_TRUE(actual_events_map.contains(event1));
  ASSERT_TRUE(actual_events_map.contains(event2));
  EXPECT_TRUE(std::holds_alternative<ymwm::environment::commands::RunTerminal>(
      actual_events_map.at(event1)));
  EXPECT_TRUE(
      std::holds_alternative<ymwm::environment::commands::ExitRequested>(
          actual_events_map.at(event2)));
}

TEST(TestEventMap, CreateEventMapFromYamlFileWithoutCmdSpecified) {
  auto events_map = ymwm::events::event_map_from_yaml(
      "key-bindings-without-cmd-specified.yaml");
  ASSERT_FALSE(std::holds_alternative<std::string>(events_map))
      << std::get<std::string>(events_map);
  const auto& actual_events_map = std::get<ymwm::events::Map>(events_map);
  const auto& default_event_map = ymwm::events::default_event_map();
  ASSERT_EQ(actual_events_map.size(), default_event_map.size());
  for (auto actual_it = actual_events_map.cbegin();
       actual_it != actual_events_map.cend();
       std::advance(actual_it, 1)) {
    ASSERT_TRUE(default_event_map.contains(actual_it->first));
    ASSERT_EQ(default_event_map.at(actual_it->first).index(),
              actual_it->second.index());
  }
}
