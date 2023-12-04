#pragma once
#pragma once
#include <plog/Record.h>
#include <plog/Util.h>

namespace plog
{
class IAppender
{
public:
    virtual ~IAppender() = default;
    virtual void write(const Record& record) = 0;
};
}
