#pragma once

#include <gkr/capi/net/lib.h>

#include <string>

namespace gkr
{
namespace net
{

inline bool lib_startup()
{
    return (gkr_net_lib_startup() == 0);
}
inline void lib_shutdown()
{
    gkr_net_lib_shutdown();
}

inline std::string get_hostname()
{
    char buf[64] {0};

    gkr_net_lib_get_hostname(buf, sizeof(buf));

    return std::string(buf);
}

struct lib final
{
    const bool initialed;

    lib() : initialed(lib_startup())
    {
    }
    ~lib()
    {
        if(initialed) lib_shutdown();
    }

private:
    lib           (const lib& other) noexcept = delete;
    lib& operator=(const lib&      ) noexcept = delete;

    lib           (lib&& other) noexcept = delete;
    lib& operator=(lib&&      ) noexcept = delete;
};

}
}
