#pragma once

#include <utility>
#include <type_traits>

#ifndef __cpp_lib_exchange_function

namespace std
{

template<class T, class U = T>
T exchange(T& obj, U&& new_value) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_assignable<T&, U>::value)
{
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
}

}

#endif
