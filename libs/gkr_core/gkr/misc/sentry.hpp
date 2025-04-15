#pragma once

#include <gkr/diagnostics.hpp>

#include <utility>

namespace gkr
{
namespace misc
{

template<typename Func>
class sentry
{
    sentry           (const sentry&) noexcept = delete;
    sentry& operator=(const sentry&) noexcept = delete;

    sentry           (sentry&&) noexcept = delete;
    sentry& operator=(sentry&&) noexcept = delete;

     Func m_func;

public:
    sentry(Func&& func) noexcept : m_func(std::move(func))
    {
    }
    ~sentry() noexcept(noexcept(m_func))
    {
        m_func();
    }
};

}
}
