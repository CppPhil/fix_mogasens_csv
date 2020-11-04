#ifndef INCG_CS_DATA_SET_INFO_HPP
#define INCG_CS_DATA_SET_INFO_HPP
#include <cstdint>

#include <stdexcept>

#include <pl/except.hpp>
#include <pl/string_view.hpp>

namespace cs {
PL_DEFINE_EXCEPTION_TYPE(NoSuchDataSetException, std::logic_error);

/*!
 * \brief Meta function for data set tags.
 * \tparam Tag The data set tag to use.
 *
 * Meta function for data set tags.
 * Contains a text for the data set tag and its repetition count.
 **/
template<typename Tag>
struct data_set_info;

#define CS_SPECIALIZE_DATA_SET_INFO(tag, string, repetitionCount)             \
  struct tag {                                                                \
  };                                                                          \
  constexpr bool contains##tag(pl::string_view other)                         \
  {                                                                           \
    return other.contains(string);                                            \
  }                                                                           \
  template<>                                                                  \
  struct data_set_info<tag> {                                                 \
    static constexpr pl::string_view text        = string;                    \
    static constexpr std::uint64_t   repetitions = UINT64_C(repetitionCount); \
  }

CS_SPECIALIZE_DATA_SET_INFO(Felix1, "11.17.39", 24);
CS_SPECIALIZE_DATA_SET_INFO(Felix2, "12.50.00", 20);
CS_SPECIALIZE_DATA_SET_INFO(Felix3, "13.00.09", 15);

CS_SPECIALIZE_DATA_SET_INFO(Marcelle1, "14.59.59", 10);
CS_SPECIALIZE_DATA_SET_INFO(Marcelle2, "15.13.22", 16);
CS_SPECIALIZE_DATA_SET_INFO(Marcelle3, "15.31.36", 18);

CS_SPECIALIZE_DATA_SET_INFO(Mike1, "14.07.33", 26);
CS_SPECIALIZE_DATA_SET_INFO(Mike2, "14.14.32", 22);
CS_SPECIALIZE_DATA_SET_INFO(Mike3, "14.20.28", 18);

CS_SPECIALIZE_DATA_SET_INFO(Andre1, "Andre_liegestuetzen1", 27);
CS_SPECIALIZE_DATA_SET_INFO(Andre2, "Andre_liegestuetzen2", 20);
CS_SPECIALIZE_DATA_SET_INFO(Andre3, "Andre_liegestuetzen3", 17);
CS_SPECIALIZE_DATA_SET_INFO(AndreSquats1, "Andre_Squats", 30);
CS_SPECIALIZE_DATA_SET_INFO(AndreSquats2, "Andre_Squats2", 49);

CS_SPECIALIZE_DATA_SET_INFO(Jan1, "Jan_liegestuetzen1", 25);
CS_SPECIALIZE_DATA_SET_INFO(Jan2, "Jan_liegestuetzen2", 19);
CS_SPECIALIZE_DATA_SET_INFO(Jan3, "Jan_liegestuetzen3", 13);

CS_SPECIALIZE_DATA_SET_INFO(Lucas1, "Lukas_liegestuetzen1", 24);
CS_SPECIALIZE_DATA_SET_INFO(Lucas2, "Lukas_liegestuetzen2", 19);
CS_SPECIALIZE_DATA_SET_INFO(Lucas3, "Lukas_liegestuetzen3", 11);

#undef CS_SPECIALIZE_DATA_SET_INFO

/*!
 * \brief Fetches the repetition count for a given data set identified by its
 *        string.
 * \param dataSet The data set to fetch the repetition count of.
 * \return The repetition count of `dataSet`.
 * \warning `dataSet` may not be invalid!
 **/
[[nodiscard]] std::uint64_t repetitionCount(pl::string_view dataSet);
} // namespace cs
#endif // INCG_CS_DATA_SET_INFO_HPP
