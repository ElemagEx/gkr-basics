#pragma once
#include <cstdarg>
#include <plog/Severity.h>
#include <plog/Util.h>

#include <gkr/log/log.hpp>

namespace plog
{
class Record : public gkr::log::ostream
{
    Record           (Record&&) noexcept = delete;
    Record& operator=(Record&&) noexcept = delete;

    Record           (const Record&) noexcept = delete;
    Record& operator=(const Record&) noexcept = delete;

    using base_t = gkr::log::ostream;
public:
    Record(Severity severity, const char* func, size_t line, const char* file, const void* object, int instanceId)
        : base_t(false, severity, instanceId), m_instanceId(instanceId), m_severity(severity), m_line(line), m_file(file), m_func(func)
    {
    }
    Record& ref()
    {
        return *this;
    }
    template<typename T>
    Record& operator<<(const T& data)
    {
        static_cast<base_t&>(*this) << data;
        return *this;
    }
    Record& operator<<(std::ostream& (*data)(std::ostream&))
    {
        static_cast<base_t&>(*this) << data;
        return *this;
    }
private:
    const int         m_instanceId;
    const Severity    m_severity;
    const std::size_t m_line;
    const char* const m_file;
    const char* const m_func;
};
}
