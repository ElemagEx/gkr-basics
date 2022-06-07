#pragma once

#include <memory>
#include <utility>
#include <type_traits>

#ifndef __cpp_lib_raw_memory_algorithms

namespace std
{

template<class InputIt, class Size, class NoThrowForwardIt>
std::pair<InputIt, NoThrowForwardIt> uninitialized_move_n(InputIt first, Size count, NoThrowForwardIt d_first)
    noexcept(std::is_nothrow_move_assignable<typename std::iterator_traits<NoThrowForwardIt>::value_type>::value)
{
    for( ; count > 0; ++d_first, (void)++first, --count)
    {
        ::new (static_cast<void*>(std::addressof(*d_first))) typename std::iterator_traits<NoThrowForwardIt>::value_type(std::move(*first));
    }
    return { first, d_first };
}
template<class ForwardIt, class Size>
ForwardIt uninitialized_default_construct_n(ForwardIt first, Size count)
    noexcept(std::is_nothrow_copy_constructible<typename std::iterator_traits<ForwardIt>::value_type>::value)
{
    using T = typename std::iterator_traits<ForwardIt>::value_type;

    for( ; count > 0 ; (void)++first, --count)
    {
        ::new (static_cast<void*>(std::addressof(*first))) T;
    }
    return first;
}

template<class T>
void destroy_at(T* ptr)
    noexcept(std::is_nothrow_destructible<T>::value)
{
    ptr->~T();
}

template<class ForwardIt, class Size>
ForwardIt destroy_n(ForwardIt first, Size count)
    noexcept(std::is_nothrow_destructible<typename std::iterator_traits<ForwardIt>::value_type>::value)
{
    for( ; count > 0; (void)++first, --count)
    {
        std::destroy_at(std::addressof(*first));
    }
    return first;
}
}

#endif
