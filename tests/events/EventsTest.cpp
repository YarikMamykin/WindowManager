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
