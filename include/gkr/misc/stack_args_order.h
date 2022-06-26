#pragma once

#include <memory>
#include <type_traits>

namespace gkr
{

namespace impl
{
static int actual_function_difference(int a, int b)
{
    return (a - b);
}
class actual_class
{
public:
    int actual_method_difference(int a, int b)
    {
        return (a - b);
    }
};

// Ignore clang and GNUC warnings
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsequence-point"
#endif
template<typename R, typename... Args>
R execute_function_proxy(R (*func)(Args...), void** params)
{

#if 1 // The correct way to call function arguments with right-to-left order in stack
    const std::size_t count = reinterpret_cast<std::size_t>(*params);

    params += count;

    R ret = (*func)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*params--)...);

#else // The correct way to call function arguments with left-to-right placement in stack

    R ret = (*func)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*++params)...);

#endif
    return ret;
}
template<typename R, typename C, typename... Args>
R execute_method_proxy(C& obj, R (C::*method)(Args...), void** params)
{
#if 1 // The correct way to call method arguments with right-to-left order in stack
    const std::size_t count = reinterpret_cast<std::size_t>(*params);

    params += count;

    R ret = (obj.*method)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*params--)...);

#else // The correct way to call method arguments with left-to-right placement in stack

    R ret = (obj.*method)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*++params)...);

#endif
    return ret;
}
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

template<typename R, typename... Args>
R execute_difference_function_remote(Args... args)
{
    constexpr std::size_t count = sizeof...(args);

    void* params[count + 1] = {reinterpret_cast<void*>(count), static_cast<void*>(std::addressof(args))...};

    return execute_function_proxy<R>(actual_function_difference, params);
}

template<typename R, typename C, typename... Args>
R execute_difference_method_remote(C& obj, Args... args)
{
    constexpr std::size_t count = sizeof...(args);

    void* params[count + 1] = {reinterpret_cast<void*>(count), static_cast<void*>(std::addressof(args))...};

    return execute_method_proxy<R>(obj, &C::actual_method_difference, params);
}

}

inline bool function_args_stack_order_is_right_to_left()
{
    const int result = impl::execute_difference_function_remote<int>(10, 1);

    return (result > 0);
}
inline bool method_args_stack_order_is_right_to_left()
{
    impl::actual_class obj;

    const int result = impl::execute_difference_method_remote<int>(obj, 10, 1);

    return (result > 0);
}

}
