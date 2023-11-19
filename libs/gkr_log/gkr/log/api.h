#pragma once

#ifdef BUILDING_GKR_LOG
#   ifdef _WIN32
#       define GKR_LOG_API __declspec(dllexport)
#   else
#       define GKR_LOG_API __attribute__((__visibility__("default")))
#   endif
#else
#   ifdef _WIN32
#       define GKR_LOG_API __declspec(dllimport)
#   else
#       define GKR_LOG_API
#   endif
#endif
