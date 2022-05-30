#pragma once

#ifdef _MSC_VER

// prevent MS C std library warnings:
//  4365 - 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
#pragma warning(disable:4365)
#include <vector>
#include <atomic>
#pragma warning(default:4365)

// prevent unsignificant warnings
#pragma warning(disable:4061) // enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
#pragma warning(disable:4100) // 'identifier' : unreferenced formal parameter
#pragma warning(disable:4514) // 'function' : unreferenced inline function has been removed
#pragma warning(disable:4820) // 'bytes' bytes padding added after construct 'member_name'
#pragma warning(disable:5045) // Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
#pragma warning(disable:5039) // 'function': pointer or reference to potentially throwing function passed to 'extern "C"' function under -EHc. Undefined behavior may occur if this function throws an exception.


//#ifndef maybe_unused
//#pragma warning(disable:5051) // attribute 'maybe_unused' requires at least '/std:c++17'; ignored
//#endif

#endif
