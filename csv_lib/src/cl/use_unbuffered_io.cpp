#include <cstdio>

#include <ios>
#include <iostream>

#include "cl/use_unbuffered_io.hpp"

namespace cl {
void useUnbufferedIo()
{
  std::setbuf(stdout, nullptr);
  std::setbuf(stderr, nullptr);
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::wcout << std::unitbuf;
  std::wcerr << std::unitbuf;
}
} // namespace cl
