#include "file.hpp"

#include <stdio.h>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace gkr
{
namespace sys
{

bool file_is_atty(void* file)
{
#ifdef _WIN32
    const int fd = _fileno(static_cast<FILE*>(file));

    return _isatty(fd)?true:false;
#else
    const int fd = fileno(static_cast<FILE*>(file));

    return isatty(fd);
#endif
}

}
}
