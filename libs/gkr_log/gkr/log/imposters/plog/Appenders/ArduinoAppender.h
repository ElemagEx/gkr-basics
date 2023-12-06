#pragma once
#include <plog/Appenders/IAppender.h>
#ifdef ARDUINO
#include <Arduino.h>
#else
typedef void* Stream;
#endif

namespace plog
{
template<class Formatter>
class ArduinoAppender : public IAppender
{
    Stream& m_stream;
public:
    ArduinoAppender(Stream& stream) : m_stream(stream) {}
    virtual void write(const Record& record) {}
    const void* data() const { return &m_stream; }
};
}
