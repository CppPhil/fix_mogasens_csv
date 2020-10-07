#ifndef INCG_CL_FS_SEPARATOR_HPP
#define INCG_CL_FS_SEPARATOR_HPP
#include <pl/os.hpp>

#if PL_OS == PL_OS_WINDOWS
#define CL_FS_SEPARATOR "\\"
#else
#define CL_FS_SEPARATOR "/"
#endif
#endif // INCG_CL_FS_SEPARATOR_HPP
