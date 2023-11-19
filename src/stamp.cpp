#include <gkr/stamp.h>

#include <gkr/diagnostics.h>

#include <chrono>
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

void gkr_stamp_decompose(int local, stamp_t stamp, struct tm* tm, unsigned* ns)
{
    Check_Arg_NotNull(tm, );

    const auto nanoseconds = std::chrono::nanoseconds(stamp);

    const auto duration = std::chrono::duration_cast<std::chrono::system_clock::duration>(nanoseconds);

    const auto when = std::chrono::system_clock::time_point(duration);

    const std::time_t time = std::chrono::system_clock::to_time_t(when);

    if(ns != nullptr) *ns = unsigned(stamp - stamp_t(time) * 1000000000LL);

#ifdef _MSC_VER
    if(local)
    {
        localtime_s(tm, &time);
    }
    else
    {
        gmtime_s(tm, &time);
    }
#else
    if(local)
    {
        localtime_r(&time, tm);
    }
    else
    {
        gmtime_r(&time, tm);
    }
#endif
}

}
