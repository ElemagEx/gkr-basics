#pragma once

#include "common.h"

namespace gkr
{

template<typename T>
class lockfree_forward_list
{
    lockfree_forward_list           (const lockfree_forward_list&) noexcept = delete;
    lockfree_forward_list& operator=(const lockfree_forward_list&) noexcept = delete;

public:
    lockfree_forward_list           (lockfree_forward_list&&) noexcept = default;
    lockfree_forward_list& operator=(lockfree_forward_list&&) noexcept = default;

    lockfree_forward_list() = default;
   ~lockfree_forward_list() = default;

public:
    T* begin()
    {
        Assert_NotImplemented();
        return nullptr;
    }
    T* end()
    {
        Assert_NotImplemented();
        return nullptr;
    }
    template<typename... Args>
    T& emplace_front(Args&&... args)
    {
        Assert_NotImplemented();
        return *(T*)nullptr;
    }
};

}
