#pragma once

#include <gkr/capi/log/log.h>

#include <utility>

namespace gkr
{
namespace log
{

struct custom_message
{
    void*    m_instance = nullptr;
    char*    m_buf      = nullptr;
    unsigned m_cch      = 0;

    custom_message           (const custom_message&) noexcept = delete;
    custom_message& operator=(const custom_message&) noexcept = delete;

    custom_message(custom_message&& other) noexcept
        : m_instance(std::exchange(other.m_instance, nullptr))
        , m_buf     (std::exchange(other.m_buf     , nullptr))
        , m_cch     (std::exchange(other.m_cch     , 0))
    {
    }
    custom_message& operator=(custom_message&& other) noexcept
    {
        m_instance  = std::exchange(other.m_instance, nullptr);
        m_buf       = std::exchange(other.m_buf     , nullptr);
        m_cch       = std::exchange(other.m_cch     , 0);
        return *this;
    }
    custom_message() noexcept
    {
    }
    custom_message(void* instance, int severity) : m_instance(instance)
    {
        gkr_log_custom_message_start(instance, severity, &m_buf, &m_cch);
    }
    ~custom_message()
    {
        cancel();
    }

    bool is_started() const noexcept
    {
        return (m_buf != nullptr);
    }
    int finish(int severity, int facility)
    {
        if(!is_started()) return 0;
        m_buf = nullptr;
        return gkr_log_custom_message_finish(m_instance, severity, facility);
    }
    int finish(const char* func, const char* file, unsigned line, int severity, int facility) {
        if(!is_started()) return 0;
        m_buf = nullptr;
        return gkr_log_custom_message_finish_ex(m_instance, func, file, line, severity, facility);
    }
    int cancel()
    {
        if(!is_started()) return -1;
        m_buf = nullptr;
        return gkr_log_custom_message_cancel(m_instance);
    }
};
}
}

#ifndef GKR_NO_STREAM_LOGGING

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
struct allocator {
    using value_type = T;
    using  size_type = std::size_t;
    using  diff_type = std::ptrdiff_t;

    bool operator==(const allocator&) const noexcept { return true ; }
    bool operator!=(const allocator&) const noexcept { return false; }

    template<class U>
    allocator(const allocator<U>&) noexcept {}
    allocator(const allocator   &) noexcept {}
    allocator(      allocator  &&) noexcept {}

    allocator() noexcept = default;
   ~allocator() noexcept = default;

#ifdef __cpp_lib_allocate_at_least
    [[nodiscard]]
    constexpr std::allocation_result<T*, std::size_t> allocate_at_least(std::size_t n) {
        std::size_t cb = n * sizeof(T);
        T* p = static_cast<T*>(allocate_bytes(cb));
        n = cb / sizeof(T);
        return {p, cb};
    }
#endif
    T* allocate(std::size_t n, const void* = nullptr) {
        std::size_t cb = n * sizeof(T);
        return static_cast<T*>(allocate_bytes(cb));
    }
    void deallocate(T* p, std::size_t n) {
        const std::size_t cb = n * sizeof(T);
        deallocate_bytes(p, cb);
    }
};
struct dummy_ostream
{
    dummy_ostream(...) {}

    template<typename T>
    dummy_ostream& operator<<(const T& data) { return *this; }
    dummy_ostream& operator<<(std::ostream& (*data)(std::ostream&)) { return *this; }
};
}

class ostream : protected custom_message
{
    using base_t = custom_message;

    using ostringstream_t = std::basic_ostringstream<char, std::char_traits<char>, impl::allocator<char>>;

    ostringstream_t m_ostream;

    ostream           (const ostream&) noexcept = delete;
    ostream& operator=(const ostream&) noexcept = delete;

protected:
    const char* m_func     = nullptr;
    const char* m_file     = nullptr;
    unsigned    m_line     = 0;
    int         m_severity = 0;
    int         m_facility = 0;

public:
    ostream(ostream&& other) noexcept(false)
        : base_t    (std::move(other))
        , m_ostream (std::move(other.m_ostream))
        , m_func    (other.m_func)
        , m_file    (other.m_file)
        , m_line    (other.m_line)
        , m_severity(other.m_severity)
        , m_facility(other.m_facility)
    {
        other.invalidate();
    }
    ostream& operator=(ostream&& other) noexcept(false)
    {
        base_t::operator=(std::move(other));

        m_ostream  = std::move(other.m_ostream );
        m_func     = other.m_func;
        m_file     = other.m_file;
        m_line     = other.m_line;
        m_severity = other.m_severity;
        m_facility = other.m_facility;

        other.invalidate();
        return *this;
    }
    ostream(void*) : base_t()
    {
        invalidate();
    }
    ostream(void* instance_, int severity, int facility)
        : base_t(instance_, severity)
        , m_ostream()
        , m_severity(severity)
        , m_facility(facility)
    {
        if(base_t::is_started())
        {
            impl::init_ostringstream_allocator(base_t::buf, base_t::cch);
        }
        else
        {
            invalidate();
        }
    }
    ostream(void* instance_, const char* func, const char* file, unsigned line, int severity, int facility)
        : base_t(instance_, severity)
        , m_ostream()
        , m_func(func)
        , m_file(file)
        , m_line(line)
        , m_severity(severity)
        , m_facility(facility)
    {
        if(base_t::is_started())
        {
            impl::init_ostringstream_allocator(base_t::buf, base_t::cch);
        }
        else
        {
            invalidate();
        }
    }
    ~ostream()
    {
        finish();
    }

public:
    int finish()
    {
        if(m_ostream.eof()) return 0;

        const auto pos = m_ostream.tellp();

        const std::size_t len = (pos < 0) ? 0 : std::size_t(pos);

        impl::done_ostringstream_allocator(std::size_t(len));

        invalidate();
        return base_t::finish(m_func, m_file, m_line, m_severity, m_facility);
    }

public:
    int operator<<(int (*cmd)(ostream&))
    {
        return (cmd)(*this);
    }
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

private:
    void invalidate()
    {
        m_ostream.setstate(std::ios_base::eofbit);
    }
};
inline int finish(ostream& os)
{
    return os.finish();
}

}
}

#endif /*GKR_NO_STREAM_LOGGING*/

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
    void seal() noexcept
    {
        if(m_ptr < m_end) *m_ptr = 0; else *(m_end - 1) = 0;
    }
private:
    T* m_ptr = nullptr;
    T* m_end = nullptr;
};
}

template<typename... Args>
int gkr_log_format_message(void* instance, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    gkr::log::custom_message message(instance, severity);
    if(!message.is_started()) return 0;

    using container_t = gkr::log::impl::container<char>;

    container_t container(message.buf, message.cch);

    auto it = std::back_inserter(container);

    std::vformat_to(std::move(it), fmt.get(), std::make_format_args(args...));

    container.seal();

    return message.finish(severity, facility);
}
template<typename... Args>
int gkr_log_format_message_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    gkr::log::custom_message message(instance, severity);
    if(!message.is_started()) return 0;

    using container_t = gkr::log::impl::container<char>;

    container_t container(message.buf, message.cch);

    auto it = std::back_inserter(container);

    std::vformat_to(std::move(it), fmt.get(), std::make_format_args(args...));

    container.seal();

    return message.finish(func, file, line, severity, facility);
}
template<typename... Args>
int gkr_log_format_message(void* instance, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    gkr::log::custom_message message(instance, severity);
    if(!message.is_started()) return 0;

    using container_t = gkr::log::impl::container<char>;

    container_t container(message.buf, message.cch);

    auto it = std::back_inserter(container);

    std::vformat_to(std::move(it), loc, fmt.get(), std::make_format_args(args...));

    container.seal();

    return message.finish(severity, facility);
}
template<typename... Args>
int gkr_log_format_message(void* instance, const char* func, const char* file, unsigned line, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    gkr::log::custom_message message(instance, severity);
    if(!message.is_started()) return 0;

    using container_t = gkr::log::impl::container<char>;

    container_t container(message.buf, message.cch);

    auto it = std::back_inserter(container);

    std::vformat_to(std::move(it), loc, fmt.get(), std::make_format_args(args...));

    container.seal();

    return message.finish(func, file, line, severity, facility);
}

#endif /*__cpp_lib_format*/
#endif /*GKR_NO_FORMAT_LOGGING*/
