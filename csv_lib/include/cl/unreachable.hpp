#ifndef INCG_CL_UNREACHABLE_HPP
#define INCG_CL_UNREACHABLE_HPP
#include <pl/compiler.hpp>

#if PL_COMPILER == PL_COMPILER_MSVC
#define CL_UNREACHABLE() __assume(0)
#elif PL_COMPILER == PL_COMPILER_GCC || PL_COMPILER == PL_COMPILER_CLANG
#define CL_UNREACHABLE() __builtin_unreachable()
#else
#error "Unsupported compiler!"
#endif
#endif // INCG_CL_UNREACHABLE_HPP
