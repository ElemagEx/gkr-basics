#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/Util.h>
#include <plog/WinApi.h>
#include <iostream>

namespace plog
{
enum OutputStream {streamStdOut=3,streamStdErr=4};
template<class Formatter>
class ConsoleAppender : public IAppender
{
    int m_outStream;
public:
    ConsoleAppender(OutputStream outStream = streamStdOut) : m_outStream(outStream) {}
    virtual void write(const Record& record) override {}
    const void* data() const { return &m_outStream; }
};
}
