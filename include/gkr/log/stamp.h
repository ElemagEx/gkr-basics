#pragma once

#include <chrono>
#include <ctime>

namespace gkr
{
namespace log
{

inline long long calc_stamp()
{
    auto now = std::chrono::system_clock::now();

    auto duration = now.time_since_epoch();

    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    long long stamp = nanoseconds.count();

    return stamp;
}

inline void decompose_stamp(bool local, long long stamp, std::tm& tm, unsigned& ns)
{
    auto nanoseconds = std::chrono::nanoseconds(stamp);

    auto duration = std::chrono::duration_cast<std::chrono::system_clock::duration>(nanoseconds);

    auto now = std::chrono::system_clock::time_point(duration);

    std::time_t time = std::chrono::system_clock::to_time_t(now);

    ns = unsigned(stamp - (long long)(time) * 1000000000LL);

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
