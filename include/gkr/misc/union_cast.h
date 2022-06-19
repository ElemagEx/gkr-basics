#pragma once

namespace gkr
{
namespace misc
{

template<typename T, typename U>
inline T union_cast(const U& value) noexcept
{
    union { T t; U u; } values {0};
    values.u = value;
    return values.t;
}

}
}
