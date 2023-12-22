#pragma once
#if defined(_MSC_VER)
extern "C" void* __cdecl _alloca(size_t size);
#define STACK_ALLOC                     __pragma(warning(suppress:6255)) _alloca
#define STACK_ARRAY(type, var, size)    __pragma(warning(suppress:6255)) type* var = (type*)_alloca((size)*sizeof(type))
#else
#define STACK_ALLOC                     __builtin_alloca
#define STACK_ARRAY(type, var, size)    type* var = (type*)__builtin_alloca((size)*sizeof(type))
#endif
