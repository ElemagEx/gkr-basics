#pragma once
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/WinApi.h>

namespace plog
{
template<class Formatter>
class ColorConsoleAppender : public ConsoleAppender<Formatter>
{
public:
    ColorConsoleAppender(OutputStream = streamStdOut) : ConsoleAppender<Formatter>(streamStdOut) {}
    virtual void write(const Record& record) override {}
};
}
