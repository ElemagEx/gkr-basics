#pragma once

#include <type_traits>

#ifndef __cpp_lib_is_swappable

namespace std
{

//TODO:Change this invalid temp implementation

template<class>
struct is_nothrow_swappable : true_type
{
};

}

#endif
