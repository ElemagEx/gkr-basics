#pragma once
#include <plog/Appenders/IAppender.h>
#include <set>

namespace plog
{
class DynamicAppender : public IAppender
{
public:
    DynamicAppender& addAppender   (IAppender* appender) { return *this; }
    DynamicAppender& removeAppender(IAppender* appender) { return *this; }

    virtual void write(const Record& record) override {}
    const void* data() const { return nullptr; }
};
}
