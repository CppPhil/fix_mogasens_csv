#include "cl/dos2unix.hpp"

namespace cl {
std::vector<pl::byte> dos2unix(const void* p, std::size_t size)
{
  using namespace pl::integer_literals;
  constexpr pl::byte carriageReturn{0x0D_byte};
  constexpr pl::byte lineFeed{0x0A_byte};

  std::vector<pl::byte> res{};
  res.reserve(size);

  const pl::byte* const begin{static_cast<const pl::byte*>(p)};
  const pl::byte* const end{begin + size};

  pl::byte previousByte{0};

  for (const pl::byte* it{begin}; it != end; ++it) {
    const pl::byte currentByte{*it};

    if (currentByte == carriageReturn) {
      previousByte = carriageReturn;
      continue;
    }

    if ((previousByte == carriageReturn) && (currentByte == lineFeed)) {
      res.push_back(lineFeed);
    }
    else {
      res.push_back(currentByte);
    }

    previousByte = currentByte;
  }

  return res;
}
} // namespace cl
