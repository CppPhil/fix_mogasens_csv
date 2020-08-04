#include "cl/dos2unix.hpp"

namespace cl {
std::vector<pl::byte> dos2unix(const void* p, std::size_t size)
{
  using namespace pl::integer_literals;
  constexpr pl::byte carriageReturn{0x0D_byte};
  constexpr pl::byte lineFeed{0x0A_byte};

  std::vector<pl::byte> result{};
  result.reserve(size);

  const pl::byte* const begin{static_cast<const pl::byte*>(p)};
  const pl::byte* const end{begin + size};

  pl::byte previousByte{0};

  for (const pl::byte* iterator{begin}; iterator != end; ++iterator) {
    const pl::byte currentByte{*iterator};

    if (currentByte == carriageReturn) {
      previousByte = carriageReturn;
      continue;
    }

    if ((previousByte == carriageReturn) && (currentByte == lineFeed)) {
      result.push_back(lineFeed);
    }
    else {
      result.push_back(currentByte);
    }

    previousByte = currentByte;
  }

  return result;
}
} // namespace cl
