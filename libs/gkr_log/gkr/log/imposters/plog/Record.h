#pragma once
#include <cstdarg>
#include <plog/Severity.h>
#include <plog/Util.h>

#include <gkr/log/log.hpp>
#include <gkr/log/consumer.hpp>

namespace plog
{
class Record : public gkr::log::ostream
{
    using base_t = gkr::log::ostream;

    Record           (const Record&) noexcept = delete;
    Record& operator=(const Record&) noexcept = delete;

private:
    const gkr::log::message* m_message = nullptr;

public:
    Record(Record&& other) noexcept : base_t(std::move(other)), m_message(other.m_message)
    {
    }
    Record& operator=(Record&& other) noexcept
    {
        base_t::operator=(std::move(other));
        m_message = other.m_message;
        return *this;
    }

    Record(Severity severity, const char* func, size_t line, const char* file, const void* object, int instanceId)
        : base_t(func, file, unsigned(line), false, severity, instanceId)
    {
    }
    Record(const gkr::log::message& message) : base_t(nullptr), m_message(&message)
    {
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
};
}
