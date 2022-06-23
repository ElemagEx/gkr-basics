#pragma once

#include <gkr/diagnostics.h>

inline const char* DIAG_GET_FORMAT(int id)
{
    if((id < 0) || (id >= DIAG_ID_COUNT))
    {
        return "Unknown diagnostic id";
    }
    static const char* s_formats[] =
    {
        "Assertion failed: %s",
        "Assertion (%s) failed",
        "Null pointer (%s)",
        "Failure: %s",
        "Failure",

    };
    return s_formats[id];
}
