#pragma once

#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Formatters/CsvFormatter.h>
#include <plog/Init.h>
#include <cstring>

namespace plog
{
template<class Formatter, int instanceId>
inline Logger<instanceId>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
{
    static RollingFileAppender<Formatter> rollingFileAppender(fileName, maxFileSize, maxFiles);
    return init<instanceId>(maxSeverity, &rollingFileAppender);
}
template<class Formatter>
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
{
    return init<Formatter, PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, fileName, maxFileSize, maxFiles);
}

namespace
{
inline bool isCsv(const char* fileName)
{
    const char* ptr = std::strrchr(fileName, '.');
    if(ptr == nullptr) return false;
    return !std::strcmp(ptr, ".csv");
}
}

template<int instanceId>
inline Logger<instanceId>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
{
    return isCsv(fileName)
        ? init<CsvFormatter, instanceId>(maxSeverity, fileName, maxFileSize, maxFiles)
        : init<TxtFormatter, instanceId>(maxSeverity, fileName, maxFileSize, maxFiles);
}
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity, const char* fileName, size_t maxFileSize = 0, int maxFiles = 0)
{
    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, fileName, maxFileSize, maxFiles);
}
}
