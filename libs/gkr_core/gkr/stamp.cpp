#include "stamp.hpp"

#include <gkr/diagnostics.h>

#include <chrono>
#include <cerrno>
#include <time.h>

extern "C" {

stamp_t gkr_stamp_now()
{
    const auto now = std::chrono::system_clock::now();

    const auto duration = now.time_since_epoch();

    const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    const stamp_t stamp = stamp_t(nanoseconds.count());

    return stamp;
}

int gkr_stamp_decompose(int local, stamp_t stamp, struct tm* tm)
{
    Check_Arg_NotNull(tm, -1);

    const auto nanoseconds = std::chrono::nanoseconds(stamp);

    const auto duration = std::chrono::duration_cast<std::chrono::system_clock::duration>(nanoseconds);

    const auto when = std::chrono::system_clock::time_point(duration);

    const std::time_t time = std::chrono::system_clock::to_time_t(when);

    const int ns = int(stamp - stamp_t(time) * 1000000000LL);

    bool has_error;
#ifdef _MSC_VER
    if(local)
    {
        has_error = (0 != localtime_s(tm, &time));
    }
    else
    {
        has_error = (0 != gmtime_s(tm, &time));
    }
#else
    if(local)
    {
        has_error = (nullptr == localtime_r(&time, tm));
    }
    else
    {
        has_error = (nullptr == gmtime_r(&time, tm));
    }
#endif
    return has_error ? -1 : ns;
}

}
