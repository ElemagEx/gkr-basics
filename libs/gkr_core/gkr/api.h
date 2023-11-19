#pragma once

#ifdef BUILDING_GKR_CORE
#   ifdef _WIN32
#       define GKR_CORE_API __declspec(dllexport)
#   else
#       define GKR_CORE_API __attribute__((__visibility__("default")))
#   endif
#else
#   ifdef _WIN32
#       define GKR_CORE_API __declspec(dllimport)
#   else
#       define GKR_CORE_API
#   endif
#endif
