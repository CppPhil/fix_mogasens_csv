#ifndef INCG_CS_DATA_SET_INFO_HPP
#define INCG_CS_DATA_SET_INFO_HPP
#include <cstdint>

#include <pl/string_view.hpp>

namespace cs {
struct Felix1 {
};
struct Felix2 {
};
struct Felix3 {
};

struct Marcelle1 {
};
struct Marcelle2 {
};
struct Marcelle3 {
};

struct Mike1 {
};
struct Mike2 {
};
struct Mike3 {
};

template<typename Tag>
struct data_set_info;

#define CS_SPECIALIZE_DATA_SET_INFO(tag, string, pushUpCount)               \
  constexpr bool is##tag(pl::string_view other) { return other == string; } \
  template<>                                                                \
  struct data_set_info<tag> {                                               \
    static constexpr pl::string_view text    = string;                      \
    static constexpr std::uint64_t   pushUps = UINT64_C(pushUpCount);       \
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

#undef CS_SPECIALIZE_DATA_SET_INFO

[[nodiscard]] std::uint64_t pushUpCount(pl::string_view dataSet);
} // namespace cs
#endif // INCG_CS_DATA_SET_INFO_HPP
