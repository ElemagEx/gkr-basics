#pragma once

#include <gkr/api.h>

namespace gkr
{
namespace net
{

GKR_INNER_API bool startup();
GKR_INNER_API void shutdown();

GKR_INNER_API int get_hostname(char* buffer, unsigned cch);

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
