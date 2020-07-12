#ifndef INCG_FMC_COLUMNS_HPP
#define INCG_FMC_COLUMNS_HPP
#include <cstddef>

namespace fmc {
inline constexpr std::size_t timeColumn{1};
inline constexpr std::size_t hardwareTimestampColumn{2};
inline constexpr std::size_t extractIdColumn{3};
inline constexpr std::size_t triggerColumn{4};
inline constexpr std::size_t accelerometerXColumn{5}; // Channel 1
inline constexpr std::size_t accelerometerYColumn{6}; // Channel 2
inline constexpr std::size_t accelerometerZColumn{7}; // Channel 3
inline constexpr std::size_t gyroscopeXColumn{8};     // Channel 4
inline constexpr std::size_t gyroscopeYColumn{9};     // Channel 5
inline constexpr std::size_t gyroscopeZColumn{10};    // Channel 6
inline constexpr std::size_t samplingRateColumn{11};
} // namespace fmc
#endif // INCG_FMC_COLUMNS_HPP
