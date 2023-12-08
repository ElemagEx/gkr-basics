#pragma once

#include <type_traits>

#ifndef __cpp_lib_is_swappable

namespace std {
template<typename T>
struct is_nothrow_swappable {
    static constexpr bool value = std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value;
};
}

#endif
