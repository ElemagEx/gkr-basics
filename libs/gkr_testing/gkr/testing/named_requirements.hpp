#pragma once

#include <type_traits>
#include <memory>

namespace gkr
{
namespace testing
{

template<typename A, typename T>
bool is_eraseable(A& allocator, T* pointer)
{
    if(pointer == nullptr) return true;

#if (__cplusplus < 201703L)
    std::allocator_traits<A>::destroy(allocator, pointer);
#else
    std::destroy_at(pointer);
#endif
    return true;
}

template<class C>
void is_container(C& c)
{
    using allocator_type  = typename C::allocator_type;
    using value_type      = typename C::value_type;
    using reference       = typename C::reference;
    using const_reference = typename C::const_reference;
    using size_type       = typename C::size_type;
    using difference_type = typename C::difference_type;

    allocator_type allocator = c.get_allocator();

    CHECK(is_eraseable<allocator_type, value_type>(allocator, nullptr));

    C c1;
    CHECK(c1.empty());

    C c2 = C();
    CHECK(c2.empty());
}

}
}
