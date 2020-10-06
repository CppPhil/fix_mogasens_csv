#ifndef INCG_CS_DATA_SET_INFO_HPP
#define INCG_CS_DATA_SET_INFO_HPP
#include <cstdint>

#include <pl/string_view.hpp>

namespace cs {
struct felix1 {
};
struct felix2 {
};
struct felix3 {
};

struct marcelle1 {
};
struct marcelle2 {
};
struct marcelle3 {
};

struct mike1 {
};
struct mike2 {
};
struct mike3 {
};

template<typename Tag>
struct data_set_info;

#define CS_SPECIALIZE_DATA_SET_INFO(tag, string, pushUpCount)         \
  template<>                                                          \
  struct data_set_info<tag> {                                         \
    static constexpr pl::string_view text    = string;                \
    static constexpr std::uint64_t   pushUps = UINT64_C(pushUpCount); \
  }

CS_SPECIALIZE_DATA_SET_INFO(felix1, "11.17.39", 24);
CS_SPECIALIZE_DATA_SET_INFO(felix2, "12.50.00", 20);
CS_SPECIALIZE_DATA_SET_INFO(felix3, "13.00.09", 15);

CS_SPECIALIZE_DATA_SET_INFO(marcelle1, "14.59.59", 10);
CS_SPECIALIZE_DATA_SET_INFO(marcelle2, "15.13.22", 16);
CS_SPECIALIZE_DATA_SET_INFO(marcelle3, "15.31.36", 18);

CS_SPECIALIZE_DATA_SET_INFO(mike1, "14.07.33", 26);
CS_SPECIALIZE_DATA_SET_INFO(mike2, "14.14.32", 22);
CS_SPECIALIZE_DATA_SET_INFO(mike3, "14.20.28", 18);

#undef CS_SPECIALIZE_DATA_SET_INFO
} // namespace cs
#endif // INCG_CS_DATA_SET_INFO_HPP
