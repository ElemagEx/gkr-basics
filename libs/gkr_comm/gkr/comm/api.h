#pragma once

#ifdef BUILDING_GKR_COMM
#   ifdef _WIN32
#       define GKR_COMM_API __declspec(dllexport)
#   else
#       define GKR_COMM_API __attribute__((__visibility__("default")))
#   endif
#else
#   ifdef _WIN32
#       define GKR_COMM_API __declspec(dllimport)
#   else
#       define GKR_COMM_API
#   endif
#endif
