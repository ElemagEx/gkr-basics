#include <gkr/defs.hpp>
#include <gkr/sys/file.hpp>

#include <gkr/diagnostics.hpp>

#include <cstdio>
#include <cstring>
#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#endif

namespace gkr
{
namespace sys
{

void file_report_error(int error, unsigned* errors)
{
    if(errors != nullptr)
    {
        unsigned err = unsigned(-1);
        if((*errors & err) != 0)
        {
            *errors = err;
            return;
        }
    }
    DIAG_VAR(const char*, text)
    std::strerror(error);
    //TODO:change Check_Failure with log
    Check_FailureMsg(text, );
}

bool file_is_atty(void* file)
{
    Check_Arg_NotNull(file, false);

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
