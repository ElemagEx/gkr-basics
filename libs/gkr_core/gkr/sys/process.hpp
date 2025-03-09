#pragma once

#include <gkr/api.hpp>

#include <string>
#include <vector>

namespace gkr
{
namespace sys
{
GKR_INNER_API unsigned get_current_process_id();

GKR_INNER_API std::string get_current_process_name();
GKR_INNER_API std::string get_current_process_path();

GKR_INNER_API std::string expand_current_process_env_vars(const char* str);

inline std::string expand_current_process_env_vars(const std::string& str)
{
    return expand_current_process_env_vars(str.c_str());
}

struct command_line
{
    std::vector<char*> args;
    std::vector<char>  buff;
};

GKR_INNER_API int get_current_process_command_line(struct command_line& cmd_line);

}
}
