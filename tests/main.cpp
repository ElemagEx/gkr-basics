#include <gkr/container/lockfree_queue.hpp>

class a {};

template<class T>
struct tst { static constexpr bool value = std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value; };

int main()
{
//  constexpr bool b1 = tst<a>::value;

    unsigned a = 5;
    unsigned b = std::exchange(a, 2U);

    return int(b);
}
