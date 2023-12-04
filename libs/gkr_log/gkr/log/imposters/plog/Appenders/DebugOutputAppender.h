#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/WinApi.h>

namespace plog
{
template<class Formatter>
class DebugOutputAppender : public IAppender
{
public:
    virtual void write(const Record& record) override {}
    const void* data() const { return nullptr; }
};
}
