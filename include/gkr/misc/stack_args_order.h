#pragma once

namespace gkr
{
namespace impl
{
inline int get_next(int& var)
{
    return ++var;
}
inline int get_diff_func(int a, int b)
{
    return (a - b);
}
struct get_diff_struct
{
    int get_diff(int a, int b)
    {
        return (a - b);
    }
};
}

inline bool function_args_stack_order_is_right_to_left()
{
    int var = 1;
    int res = impl::get_diff_func(impl::get_next(var), impl::get_next(var));

    return (res > 0);
}
inline bool method_args_stack_order_is_right_to_left()
{
    int var = 1;
    int res = impl::get_diff_struct{}.get_diff(impl::get_next(var), impl::get_next(var));

    return (res > 0);
}

}
