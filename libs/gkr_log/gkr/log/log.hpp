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
        const std::size_t cb = n * sizeof(T);
        deallocate_bytes(p, cb);
    }
};
}

class ostream
{
    using ostringstream = std::basic_ostringstream<char, std::char_traits<char>, impl::allocator<char>>;

    ostringstream m_ostream;

protected:
    const char*   m_func     = nullptr;
    const char*   m_file     = nullptr;
    unsigned      m_line     = 0;
    int           m_wait     = 0;
    int           m_severity = 0;
    int           m_facility = 0;

private:
    ostream           (const ostream&) noexcept = delete;
    ostream& operator=(const ostream&) noexcept = delete;

public:
    ostream(ostream&& other) noexcept
        : m_ostream (std::move(other.m_ostream))
        , m_func    (other.m_func)
        , m_file    (other.m_file)
        , m_line    (other.m_line)
        , m_wait    (other.m_wait)
        , m_severity(other.m_severity)
        , m_facility(other.m_facility)
    {
        other.m_ostream.setstate(std::ios_base::eofbit);
    }
    ostream& operator=(ostream&& other) noexcept
    {
        m_ostream  = std::move(other.m_ostream );
        m_func     = other.m_func;
        m_file     = other.m_file;
        m_line     = other.m_line;
        m_wait     = other.m_wait;
        m_severity = other.m_severity;
        m_facility = other.m_facility;

        other.m_ostream.setstate(std::ios_base::eofbit);
        return *this;
    }
    ostream(void*)
    {
        m_ostream.setstate(std::ios_base::eofbit);
    }
    ostream(int wait, int severity, int facility)
        : m_ostream()
        , m_wait(wait)
        , m_severity(severity)
        , m_facility(facility)
    {
        char* buf;
        unsigned cch;
        if(gkr_log_custom_message_start(&buf, &cch))
        {
            impl::init_ostringstream_allocator(buf, cch);
        }
        else
        {
            m_ostream.setstate(std::ios_base::eofbit);
        }
    }
    ostream(const char* func, const char* file, unsigned line, int wait, int severity, int facility)
        : m_ostream()
        , m_func(func)
        , m_file(file)
        , m_line(line)
        , m_wait(wait)
        , m_severity(severity)
        , m_facility(facility)
    {
        char* buf;
        unsigned cch;
        if(gkr_log_custom_message_start(&buf, &cch))
        {
            impl::init_ostringstream_allocator(buf, cch);
        }
        else
        {
            m_ostream.setstate(std::ios_base::eofbit);
        }
    }
    ~ostream()
    {
        if(m_ostream.eof()) return;

        const auto pos = m_ostream.tellp();

        const std::size_t len = (pos < 0) ? 0 : std::size_t(pos);

        impl::done_ostringstream_allocator(std::size_t(len));

        gkr_log_custom_message_finish_ex(m_func, m_file, m_line, m_wait, m_severity, m_facility);
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

namespace gkr::log::impl
{
template<typename T>
class container
{
    container           (const container&) noexcept = delete;
    container& operator=(const container&) noexcept = delete;
public:
    using value_type = T;

    container           (container&& other) noexcept : m_ptr(other.m_ptr), m_end(other.m_end) {}
    container& operator=(container&& other) noexcept { m_ptr=other.m_ptr;  m_end=other.m_end; return *this; }

    container(T* ptr, const std::size_t cap) noexcept : m_ptr(ptr), m_end(m_ptr+cap) {}
   ~container() noexcept = default;

    void push_back(const T& value) noexcept
    {
        if(m_ptr < m_end) *m_ptr = value;
        ++m_ptr;
    }
    void seal()
    {
        if(m_ptr < m_end) *m_ptr = 0; else *(m_end - 1) = 0;
    }
private:
    T* m_ptr = nullptr;
    T* m_end = nullptr;
};
}

template<typename... Args>
bool gkr_log_format_message(int wait, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    char* buf;
    unsigned cch;
    if(!gkr_log_custom_message_start(&buf, &cch)) return false;

    using container_t = gkr::log::impl::container<char>;

    container_t container(buf, cch);

    auto it = std::back_inserter(container);

    std::vformat_to(std::move(it), fmt.get(), std::make_format_args(args...));

    container.seal();

    return !!gkr_log_custom_message_finish(wait, severity, facility);
}
template<typename... Args>
bool gkr_log_format_message_ex(const char* func, const char* file, unsigned line, int wait, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    char* buf;
    unsigned cch;
    if(!gkr_log_custom_message_start(&buf, &cch)) return false;

    using container_t = gkr::log::impl::container<char>;

    container_t container(buf, cch);

    auto it = std::back_inserter(container);

    std::vformat_to(std::move(it), fmt.get(), std::make_format_args(args...));

    container.seal();

    return !!gkr_log_custom_message_finish_ex(func, file, line, wait, severity, facility);
}
template<typename... Args>
bool gkr_log_format_message(int wait, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    char* buf;
    unsigned cch;
    if(!gkr_log_custom_message_start(&buf, &cch)) return false;

    using container_t = gkr::log::impl::container<char>;

    container_t container(buf, cch);

    auto it = std::back_inserter(container);

    std::vformat_to(std::move(it), loc, fmt.get(), std::make_format_args(args...));

    container.seal();

    return !!gkr_log_custom_message_finish(wait, severity, facility);
}
template<typename... Args>
bool gkr_log_format_message(const char* func, const char* file, unsigned line, int wait, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    char* buf;
    unsigned cch;
    if(!gkr_log_custom_message_start(&buf, &cch)) return false;

    using container_t = gkr::log::impl::container<char>;

    container_t container(buf, cch);

    auto it = std::back_inserter(container);

    std::vformat_to(std::move(it), loc, fmt.get(), std::make_format_args(args...));

    container.seal();

    return !!gkr_log_custom_message_finish_ex(func, file, line, wait, severity, facility);
}

#endif
#endif
