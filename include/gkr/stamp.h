#pragma once

#include <chrono>
#include <ctime>

namespace gkr
{
namespace log
{

using stamp_t = long long;

inline stamp_t calc_stamp()
{
    const auto now = std::chrono::system_clock::now();

    const auto duration = now.time_since_epoch();

    const auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    const stamp_t stamp = stamp_t(nanoseconds.count());

    return stamp;
}

inline void decompose_stamp(bool local, stamp_t stamp, std::tm& tm, unsigned& ns)
{
    const auto nanoseconds = std::chrono::nanoseconds(stamp);

    const auto duration = std::chrono::duration_cast<std::chrono::system_clock::duration>(nanoseconds);

    const auto when = std::chrono::system_clock::time_point(duration);

    const std::time_t time = std::chrono::system_clock::to_time_t(when);

    ns = unsigned(stamp - stamp_t(time) * 1000000000LL);

#ifdef _MSC_VER
    if(local)
    {
        localtime_s(&tm, &time);
    }
    else
    {
        gmtime_s(&tm, &time);
    }
#else
    if(local)
    {
        localtime_r(&time, &tm);
    }
    else
    {
        gmtime_r(&time, &tm);
    }
#endif
}

}
}
