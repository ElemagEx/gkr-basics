#pragma once

#ifndef GKR_NET_API
#define GKR_NET_API
#endif

namespace gkr
{
namespace net
{

GKR_NET_API bool startup();
GKR_NET_API void shutdown();

GKR_NET_API int get_hostname(char* buffer, unsigned cch);

struct lib
{
    const bool initialed;

    lib() : initialed(startup())
    {
    }
    ~lib()
    {
        if(initialed) shutdown();
    }

    lib           (lib&& other) noexcept : initialed(other.initialed) {}
    lib& operator=(lib&&      ) noexcept { return *this; }

    lib           (const lib& other) noexcept : initialed(other.initialed) {}
    lib& operator=(const lib&      ) noexcept { return *this; }
};

}
}
