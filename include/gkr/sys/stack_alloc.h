#pragma once

#if defined(_MSC_VER)
extern "C" void* __cdecl _alloca(size_t size);
#define stack_alloc __pragma(warning(suppress:6255 6263)) _alloca
#else
#define stack_alloc __builtin_alloca
#endif
