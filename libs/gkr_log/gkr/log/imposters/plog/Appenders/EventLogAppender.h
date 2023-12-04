#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/WinApi.h>

namespace plog
{
template <class Formatter>
class EventLogAppender : public IAppender
{
    char* m_sourceName;
public:
    EventLogAppender(const char* sourceName) : m_sourceName(sourceName) {}
    virtual void write(const Record& record) override {}
    const void* data() const { return m_sourceName; }
};
}
