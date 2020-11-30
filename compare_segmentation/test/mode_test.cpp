#include "gtest/gtest.h"

#include <cl/to_string.hpp>

#include "mode.hpp"

TEST(Mode, shouldPrintCorrectly)
{
  using namespace std::string_literals;

  EXPECT_EQ("AllDataSets"s, cl::to_string(cs::Mode::AllDataSets));
  EXPECT_EQ("AllPushUps"s, cl::to_string(cs::Mode::AllPushUps));
  EXPECT_EQ("PushUps250Hz"s, cl::to_string(cs::Mode::PushUps250Hz));
  EXPECT_EQ("PushUps200Hz"s, cl::to_string(cs::Mode::PushUps200Hz));
  EXPECT_EQ("Squats"s, cl::to_string(cs::Mode::Squats));
}

TEST(Mode, shouldParseCorrectly)
{
  EXPECT_EQ(cs::Mode::AllDataSets, cs::parseMode("AllDataSets"));
  EXPECT_EQ(cs::Mode::AllPushUps, cs::parseMode("AllPushUps"));
  EXPECT_EQ(cs::Mode::PushUps250Hz, cs::parseMode("PushUps250Hz"));
  EXPECT_EQ(cs::Mode::PushUps200Hz, cs::parseMode("PushUps200Hz"));
  EXPECT_EQ(cs::Mode::Squats, cs::parseMode("Squats"));
}
