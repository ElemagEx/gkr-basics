#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/Converters/UTF8Converter.h>
#include <plog/Converters/NativeEOLConverter.h>
#include <plog/Util.h>
#include <algorithm>

namespace plog
{

struct RollingFileData {const char* fileName; size_t maxFileSize; int maxFiles; };

template<class Formatter, class Converter = NativeEOLConverter<UTF8Converter> >
class RollingFileAppender : public IAppender
{
    RollingFileData m_data;
public:
    RollingFileAppender(const char* fileName, size_t maxFileSize = 0, int maxFiles = 0) : m_data{fileName, maxFileSize, maxFiles} {}
    virtual void write(const Record& record) override {}
    const void* data() const { return &m_data; }
};
}
