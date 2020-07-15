#include "gtest/gtest.h"

#include <pl/string_view.hpp>

#include "data_set.hpp"
#include "read_csv_file.hpp"

// TODO: DEBUG
#include <iostream>
// TODO: DEBUG

namespace {
constexpr char csvFilePath[] = "csv_lib/test/resources/data_set.csv";

cl::Expected<std::vector<std::vector<std::string>>> read()
{
  return cl::readCsvFile(csvFilePath);
}
} // namespace

TEST(DataSet, shouldBeAbleToCreateFromValidData)
{
  const cl::Expected<std::vector<std::vector<std::string>>> expected{read()};

  ASSERT_TRUE(expected.has_value());

  const std::vector<std::vector<std::string>>& dataRead{expected.value()};

  const cl::Expected<cl::DataSet> dataSetExpected{
    cl::DataSet::create(csvFilePath, dataRead)};

  ASSERT_TRUE(dataSetExpected.has_value());

  const cl::DataSet& dataSet{dataSetExpected.value()};

  EXPECT_EQ(csvFilePath, dataSet.fileName());
  EXPECT_EQ(2, dataSet.rowCount());

#ifndef EXPECT_LONG_DOUBLE_EQ
#define EXPECT_LONG_DOUBLE_EQ(a, b) \
  EXPECT_DOUBLE_EQ(static_cast<double>(a), static_cast<double>(b))
#endif

  // TODO: DEBUG
  std::cerr << "ABOUT TO DO IT > >A :<><:>>>>>\n";
  // TODO: DEBUG

  EXPECT_LONG_DOUBLE_EQ(
    0.47111L, dataSet.accelerometerAverage(cl::Sensor::LeftArm));
  EXPECT_LONG_DOUBLE_EQ(
    9.95934966666666666741L, dataSet.gyroscopeAverage(cl::Sensor::LeftArm));

  EXPECT_LONG_DOUBLE_EQ(0.0, dataSet.time(0));
  EXPECT_EQ(64403, dataSet.hardwareTimestamp(0));
  EXPECT_EQ(cl::Sensor::LeftArm, dataSet.extractId(0));
  EXPECT_LONG_DOUBLE_EQ(0.0, dataSet.trigger(0));
  EXPECT_LONG_DOUBLE_EQ(-0.898560, dataSet.accelerometerX(0));
  EXPECT_LONG_DOUBLE_EQ(0.389221, dataSet.accelerometerY(0));
  EXPECT_LONG_DOUBLE_EQ(0.157776, dataSet.accelerometerZ(0));
  EXPECT_LONG_DOUBLE_EQ(-9.695122, dataSet.gyroscopeX(0));
  EXPECT_LONG_DOUBLE_EQ(18.902439, dataSet.gyroscopeY(0));
  EXPECT_LONG_DOUBLE_EQ(6.829268, dataSet.gyroscopeZ(0));

  EXPECT_LONG_DOUBLE_EQ(0.001, dataSet.time(1));
  EXPECT_EQ(64403, dataSet.hardwareTimestamp(1));
  EXPECT_EQ(cl::Sensor::LeftArm, dataSet.extractId(1));
  EXPECT_LONG_DOUBLE_EQ(0.0, dataSet.trigger(1));
  EXPECT_LONG_DOUBLE_EQ(-0.436462, dataSet.accelerometerX(1));
  EXPECT_LONG_DOUBLE_EQ(-0.008423, dataSet.accelerometerY(1));
  EXPECT_LONG_DOUBLE_EQ(-0.936218, dataSet.accelerometerZ(1));
  EXPECT_LONG_DOUBLE_EQ(12.012195, dataSet.gyroscopeX(1));
  EXPECT_LONG_DOUBLE_EQ(-8.963415, dataSet.gyroscopeY(1));
  EXPECT_LONG_DOUBLE_EQ(-3.353659, dataSet.gyroscopeZ(1));
}

TEST(DataSet, shouldNotBeAbleToCreateFromEmtyMatrix)
{
  const cl::Expected<cl::DataSet> dataSet{cl::DataSet::create("file.csv", {})};

  ASSERT_FALSE(dataSet.has_value());

  const cl::Error& error{dataSet.error()};

  EXPECT_EQ(cl::Error::InvalidArgument, error.kind());
  EXPECT_EQ("The matrix is empty.", error.message());
}

TEST(DataSet, shouldNotBeAbleToCreateFromJaggedMatrix)
{
  const std::vector<std::vector<std::string>> data{
    {"0.01",
     "1234",
     "770",
     "0",
     "1.5",
     "1.5",
     "1.5",
     "100.0",
     "100.0",
     "100.0"},
    {"0.20"}};

  const cl::Expected<cl::DataSet> dataSet{
    cl::DataSet::create("test.csv", data)};

  ASSERT_FALSE(dataSet.has_value());

  const cl::Error& error{dataSet.error()};

  EXPECT_EQ(cl::Error::InvalidArgument, error.kind());
  EXPECT_EQ(
    "One or more rows in the matrix don't have a length of 10.",
    error.message());
}

TEST(DataSet, shouldNotBeAbleToCreateFromInvalidData)
{
  const std::vector<std::vector<std::string>> data{
    {"0.01",
     "1234",
     "770",
     "0",
     "1.5",
     "1.5",
     "1.5",
     "MOINMOIN",
     "100.0",
     "100.2"}};

  const cl::Expected<cl::DataSet> dataSet{
    cl::DataSet::create("test.csv", data)};

  ASSERT_FALSE(dataSet.has_value());

  const cl::Error& error{dataSet.error()};

  EXPECT_EQ(cl::Error::InvalidArgument, error.kind());
  EXPECT_TRUE(pl::string_view{error.message()}.starts_with(
    "Conversion failure (std::invalid_argument caught):"));
}
