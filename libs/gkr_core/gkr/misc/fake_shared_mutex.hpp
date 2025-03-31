#pragma once

namespace gkr
{
namespace misc
{

class fake_shared_mutex
{
public:
    fake_shared_mutex() noexcept
    {
    }
    ~fake_shared_mutex() noexcept
    {
    }

public:
    void lock() noexcept
    {
    }
    void unlock() noexcept
    {
    }
    bool try_lock() noexcept
    {
        return true;
    }

public:
    void lock_shared() noexcept
    {
    }
    void unlock_shared() noexcept
    {
    }
    bool try_lock_shared() noexcept
    {
        return true;
    }
};

}
}
