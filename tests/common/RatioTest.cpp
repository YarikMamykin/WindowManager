#include "common/Ratio.h"

#include <gtest/gtest.h>
#include <ranges>

TEST(TestRatio, InitWithInBoundValue) {
  for (const auto v : std::ranges::views::iota(0u, 100u)) {
    unsigned int value = ymwm::common::Ratio{ v };
    ASSERT_EQ(v, value);
  }
}

TEST(TestRatio, InitWithOverlappingValue) {
  unsigned int value = ymwm::common::Ratio{ 101 };
  EXPECT_EQ(100, value);
}
