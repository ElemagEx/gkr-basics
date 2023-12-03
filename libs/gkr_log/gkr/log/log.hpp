#pragma once

#include <gkr/log/log.h>

#ifndef GKR_NO_OSTREAM_LOGGING

#include <memory>
#include <sstream>

namespace gkr
{
namespace log
{
namespace impl
{
GKR_LOG_API void init_ostringstream_allocator(char* buf, std::size_t cch);
GKR_LOG_API void done_ostringstream_allocator(std::size_t len);
GKR_LOG_API void* allocate_bytes(std::size_t& cb);
GKR_LOG_API void deallocate_bytes(void* ptr, std::size_t cb);

template<typename T>
struct allocator
{
    using value_type = T;
    using  size_type = std::size_t;
    using  diff_type = std::ptrdiff_t;

    bool operator==(const allocator&) const noexcept { return true; }
    bool operator!=(const allocator&) const noexcept { return false; }

    template<class U>
    allocator(const allocator<U>&) noexcept {}
    allocator(const allocator   &) noexcept {}
    allocator(      allocator  &&) noexcept {}

    allocator() noexcept = default;
   ~allocator() noexcept = default;

#ifdef __cpp_lib_allocate_at_least
    [[nodiscard]]
    constexpr std::allocation_result<T*, std::size_t> allocate_at_least(std::size_t n)
    {
        std::size_t cb = n * sizeof(T);
        T* p = static_cast<T*>(allocate_bytes(cb));
        n = cb / sizeof(T);
        return std::allocation_result{p, cb};
    }
#endif
    T* allocate(std::size_t n, const void* = nullptr)
    {
        std::size_t cb = n * sizeof(T);
        return static_cast<T*>(allocate_bytes(cb));
    }
    void deallocate(T* p, std::size_t n)
    {
        std::size_t cb = n * sizeof(T);
        deallocate_bytes(p, cb);
    }
};
}

class ostream
{
    using ostringstream = std::basic_ostringstream<char, std::char_traits<char>, impl::allocator<char>>;

    ostringstream m_ostream;
    int m_wait;
    int m_severity;
    int m_facility;

private:
    ostream           (const ostream&) noexcept = delete;
    ostream& operator=(const ostream&) noexcept = delete;

public:
    ostream(ostream&& other) noexcept
        : m_ostream (std::move(other.m_ostream))
        , m_wait    (other.m_wait    )
        , m_severity(other.m_severity)
        , m_facility(other.m_facility)
    {
        other.m_ostream.setstate(std::ios_base::eofbit);
    }
    ostream& operator=(ostream&& other) noexcept
    {
        m_ostream  = std::move(other.m_ostream );
        m_wait     = other.m_wait    ;
        m_severity = other.m_severity;
        m_facility = other.m_facility;

        other.m_ostream.setstate(std::ios_base::eofbit);
        return *this;
    }
    ostream(int wait, int severity, int facility)
        : m_ostream()
        , m_wait(wait)
        , m_severity(severity)
        , m_facility(facility)
    {
        char* buf;
        unsigned cch;
        if(gkr_log_message_start(&buf, &cch))
        {
            impl::init_ostringstream_allocator(buf, cch);
        }
        else
        {
            m_ostream.setstate(std::ios_base::badbit);
        }
    }
    ~ostream()
    {
        if(m_ostream.bad() || m_ostream.eof()) return;

        const auto pos = m_ostream.tellp();

        const std::size_t len = (pos < 0) ? 0 : std::size_t(pos);

        impl::done_ostringstream_allocator(std::size_t(len));

        gkr_log_message_finish(m_wait, m_severity, m_facility);
    }

public:
    ostream& operator<<(std::ostream& (*data)(std::ostream&))
    {
        m_ostream << data;
        return *this;
    }
    template<typename T>
    ostream& operator<<(const T& data)
    {
        m_ostream << data;
        return *this;
    }
};

}
}

#endif

#ifndef GKR_NO_FORMAT_LOGGING
#ifdef __cpp_lib_format

#include <format>

namespace gkr
{
namespace log
{



}
}

#endif
#endif
