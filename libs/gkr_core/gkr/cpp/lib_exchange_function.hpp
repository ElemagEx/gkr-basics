#pragma once

#include <utility>

#ifndef __cpp_lib_exchange_function

#include <type_traits>

namespace std {
template<class T, class U=T>
T exchange(T& obj, U&& new_value) noexcept(std::is_nothrow_move_constructible<T>::value && std::is_nothrow_assignable<T&, U>::value) {
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}
}

#endif
