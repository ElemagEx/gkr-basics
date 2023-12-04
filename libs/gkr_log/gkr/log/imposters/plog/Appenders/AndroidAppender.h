#pragma once
#include <plog/Appenders/IAppender.h>
#ifdef __ANDROID__
#include <android/log.h>
#endif

namespace plog
{
template<class Formatter>
class AndroidAppender : public IAppender
{
    const char* const m_tag;
public:
    AndroidAppender(const char* tag) : m_tag(tag) {}
    virtual void write(const Record& record) override {}
    const void* data() const { return m_tag; }
};
}
