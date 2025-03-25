#pragma once

#include <gkr/api.hpp>

#include <string>

namespace gkr
{
namespace net
{

GKR_INNER_API bool startup();
GKR_INNER_API void shutdown();

GKR_INNER_API std::string get_hostname();

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

    lib           (lib&& other) noexcept : initialed(other.initialed) {}
    lib& operator=(lib&&      ) noexcept { return *this; }

private:
    lib           (const lib& other) noexcept = delete;
    lib& operator=(const lib&      ) noexcept = delete;
};

}
}
