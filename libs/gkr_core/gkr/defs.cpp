#include <gkr/defs.hpp>
#include <gkr/diagnostics.hpp>

#include <atomic>

namespace
{
void gkr_diag_report(int, const char*, const char*, const char*, int)
{
}

std::atomic<GKR_DIAG_REPORT_FUNC> s_reportFunction { gkr_diag_report };
}

extern "C" {

GKR_DIAG_REPORT_FUNC gkr_hook_diag_report_func(GKR_DIAG_REPORT_FUNC fn)
{
    Check_Arg_NotNull(fn, nullptr);
    return s_reportFunction.exchange(fn);
}

[[noreturn]]
void gkr_diag_halt(int id, const char* text, const char* func, const char* file, int line)
{
    (*s_reportFunction.load())(id, text, func, file, line);
    diag_trap();
}

int gkr_diag_warn(int id, const char* text, const char* func, const char* file, int line)
{
    (*s_reportFunction.load())(id, text, func, file, line);
    return 1;
}

}
