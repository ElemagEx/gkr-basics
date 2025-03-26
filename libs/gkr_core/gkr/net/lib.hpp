#pragma once

#include <gkr/capi/net/lib.h>

#include <string>

namespace gkr
{
namespace net
{

bool startup()
{
    return (gkr_net_lib_startup() == 0);
}
void shutdown()
{
    gkr_net_lib_shutdown();
}

std::string get_hostname()
{
    char buf[64] {0};

    gkr_net_lib_get_hostname(buf, sizeof(buf));

    return std::string(buf);
}

struct lib final
{
    const bool initialed;

    lib() : initialed(startup())
    {
    }
    ~lib()
    {
        if(initialed) shutdown();
    }

private:
    lib           (const lib& other) noexcept = delete;
    lib& operator=(const lib&      ) noexcept = delete;

    lib           (lib&& other) noexcept = delete;
    lib& operator=(lib&&      ) noexcept = delete;
};

}
}
