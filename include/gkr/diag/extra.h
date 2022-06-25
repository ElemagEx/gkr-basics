#pragma once

#include <gkr/diag/diagnostics.h>

inline const char* DIAG_GET_PREFIX(int id)
{
    if((id < 0) || (id >= DIAG_ID_COUNT))
    {
        return "Unknown diagnostic id";
    }
    static const char* s_prefixes[] =
    {
        "NULL Pointer Assertion fail: ",
        "Assertion fail: ",
        "Assertion: ",
        "Assertion Failure: ",
        "Assertion General Failure",
        "NULL Pointer Check fail: ",
        "Valid State Check fail: ",
        "Failure: ",
        "General Failure",
        "Failure with Recovery: "
        "Argument Check Fail: ",
        "Argument NULL Pointer: ",
        "Invalid Argument: ",
        "Invalid Array Argument: ",
    };
    return s_formats[id];
}
