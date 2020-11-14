#include <ostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

#include <pl/string_view.hpp>
#include <pl/unreachable.hpp>

#include <cl/exception.hpp>

#include "data_set_identifier.hpp"

namespace cm {
namespace {
pl::string_view toString(DataSetIdentifier dsi)
{
  switch (dsi) {
#define CM_DATA_SET_IDENTIFIER_X(enm) \
  case DataSetIdentifier::enm: return #enm;
    CM_DATA_SET_IDENTIFIER
#undef CM_DATA_SET_IDENTIFIER
  }

  PL_UNREACHABLE();
}
} // namespace

std::ostream& operator<<(std::ostream& os, DataSetIdentifier dsi)
{
  return os << toString(dsi);
}

DataSetIdentifier toDataSetIdentifier(const cl::fs::Path& path)
{
  const pl::string_view sv{path.str()};

#define DSI DataSetIdentifier
  if (sv.contains("11.17.39")) { return DSI::Felix_11_17_39; }
  if (sv.contains("12.50.00")) { return DSI::Felix_12_50_00; }
  if (sv.contains("13.00.09")) { return DSI::Felix_13_00_09; }
  if (sv.contains("14.07.33")) { return DSI::Mike_14_07_33; }
  if (sv.contains("14.14.32")) { return DSI::Mike_14_14_32; }
  if (sv.contains("14.20.28")) { return DSI::Mike_14_20_28; }
  if (sv.contains("14.59.59")) { return DSI::Marsi_14_59_59; }
  if (sv.contains("15.13.22")) { return DSI::Marsi_15_13_22; }
  if (sv.contains("15.31.36")) { return DSI::Marsi_15_31_36; }
  if (sv.contains("Andre_Squats2")) { return DSI::Andre_Squats_2; }
  if (sv.contains("Andre_Squats") && !sv.contains("Andre_Squats2")) {
    return DSI::Andre_Squats_1;
  }
  if (sv.contains("Andre_liegestuetzen1")) { return DSI::Andre_1; }
  if (sv.contains("Andre_liegestuetzen2")) { return DSI::Andre_2; }
  if (sv.contains("Andre_liegestuetzen3")) { return DSI::Andre_3; }
  if (sv.contains("Jan_liegestuetzen1")) { return DSI::Jan_1; }
  if (sv.contains("Jan_liegestuetzen2")) { return DSI::Jan_2; }
  if (sv.contains("Jan_liegestuetzen3")) { return DSI::Jan_3; }
  if (sv.contains("Lukas_liegestuetzen1")) { return DSI::Lucas_1; }
  if (sv.contains("Lukas_liegestuetzen2")) { return DSI::Lucas_2; }
  if (sv.contains("Lukas_liegestuetzen3")) { return DSI::Lucas_3; }

#undef DSI

  CL_THROW_FMT("Unknown data set: \"{}\"!", path);
}
} // namespace cm
