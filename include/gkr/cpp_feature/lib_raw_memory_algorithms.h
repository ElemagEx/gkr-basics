#pragma once

#ifndef __cpp_lib_raw_memory_algorithms

namespace std
{

template<class InputIt, class Size, class NoThrowForwardIt>
std::pair<InputIt, NoThrowForwardIt> uninitialized_move_n(InputIt first, Size count, NoThrowForwardIt d_first)
{
    for( ; count > 0; ++d_first, (void) ++first, --count)
    {
        ::new (static_cast<void*>(std::addressof(*d_first))) typename std::iterator_traits<NoThrowForwardIt>::value_type(std::move(*first));
    }
    return { first, d_first };
}

};

#endif
