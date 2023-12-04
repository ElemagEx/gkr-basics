#pragma once

#include <plog/Record.h>
#include <plog/Util.h>

namespace plog
{
class MessageOnlyFormatter
{
public:
    static std::string header(){return"";}
    static std::string format(const Record& record){return"";}
};
}
