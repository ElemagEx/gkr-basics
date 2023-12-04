#pragma once
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Init.h>

namespace plog
{

template<class Formatter, int instanceId>
inline Logger<instanceId>& init(Severity maxSeverity, OutputStream outputStream)
{
    static ColorConsoleAppender<Formatter> appender(outputStream);
    auto& logger = init<instanceId>(maxSeverity, &appender);
    return logger;
}
template<class Formatter>
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, OutputStream outputStream)
{
    return init<Formatter, PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, outputStream);
}
}
