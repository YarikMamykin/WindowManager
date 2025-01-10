#include "common/Color.h"

#include <gtest/gtest.h>

TEST(ColorTest, CreateDefault) {
  auto c = ymwm::common::Color();
  EXPECT_EQ(0u, c.red);
  EXPECT_EQ(0u, c.green);
  EXPECT_EQ(0u, c.blue);
  EXPECT_EQ(0ul, static_cast<std::size_t>(c));
}

TEST(ColorTest, CreateWithRGBComponents) {
  auto c = ymwm::common::Color(0xcc, 0xff, 0xdd);
  EXPECT_EQ(0xcc, c.red);
  EXPECT_EQ(0xff, c.green);
  EXPECT_EQ(0xdd, c.blue);
  std::size_t expected_color_long = 0xcc;
  expected_color_long <<= sizeof(unsigned short) * 8;
  expected_color_long |= 0xff;
  expected_color_long <<= sizeof(unsigned short) * 8;
  expected_color_long |= 0xdd;
  EXPECT_EQ(expected_color_long, static_cast<std::size_t>(c));
}

TEST(ColorTest, CreateWithLong) {
  std::size_t expected_color_long = 0xcc;
  expected_color_long <<= sizeof(unsigned short) * 8;
  expected_color_long |= 0xff;
  expected_color_long <<= sizeof(unsigned short) * 8;
  expected_color_long |= 0xdd;
  auto c = ymwm::common::Color(expected_color_long);
  EXPECT_EQ(0xcc, c.red);
  EXPECT_EQ(0xff, c.green);
  EXPECT_EQ(0xdd, c.blue);
  EXPECT_EQ(expected_color_long, static_cast<std::size_t>(c));
}

TEST(ColorTest, FromHexString) {
  std::size_t expected_color_long = 0xcc;
  expected_color_long <<= sizeof(unsigned short) * 8;
  expected_color_long |= 0xff;
  expected_color_long <<= sizeof(unsigned short) * 8;
  expected_color_long |= 0xdd;

  auto c = ymwm::common::Color("#ccffdd");
  EXPECT_EQ(0xcc, c.red);
  EXPECT_EQ(0xff, c.green);
  EXPECT_EQ(0xdd, c.blue);
  EXPECT_EQ(expected_color_long, static_cast<std::size_t>(c));
}
