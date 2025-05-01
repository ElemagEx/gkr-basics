#pragma once

#include <gkr/capi/url.h>

#include <gkr/diagnostics.hpp>
#include <gkr/container/raw_buffer.hpp>

namespace gkr
{

using url_part = gkr_url_part;
using url_data = gkr_url_data;

template<typename Allocator>
class basic_url
{
    using buff_t = basic_raw_buffer<Allocator>;

    buff_t   m_buff;
    url_data m_data {};

public:
    basic_url() noexcept(std::is_nothrow_default_constructible<buff_t>::value) = default;
   ~basic_url() noexcept(std::is_nothrow_destructible         <buff_t>::value) = default;

    basic_url(basic_url&& other) noexcept(std::is_nothrow_move_constructible<buff_t>::value)
        : m_buff(std::move    (other.m_buff))
        , m_data(std::exchange(other.m_data, {}))
    {
    }
    basic_url(const basic_url& other) noexcept(std::is_nothrow_copy_constructible<buff_t>::value)
        : m_buff(other.m_buff)
        , m_data(other.m_data)
    {
    }
    basic_url& operator=(basic_url&& other) noexcept(std::is_nothrow_move_assignable<buff_t>::value)
    {
        m_buff = std::move    (other.m_buff);
        m_data = std::exchange(other.m_data, {});
        return *this;
    }
    basic_url& operator=(const basic_url& other) noexcept(std::is_nothrow_copy_assignable<buff_t>::value)
    {
        m_buff = other.m_buff;
        m_data = other.m_data;
        return *this;
    }

public:
    basic_url(std::size_t capacity) : m_buff(capacity)
    {
    }
    basic_url(const char* str, bool unescape = false) : m_buff()
    {
        assign(str, unescape);
    }
    basic_url(std::size_t capacity, const char* str, bool unescape = false) : m_buff(capacity)
    {
        assign(str, unescape);
    }

public:
    bool is_empty() const
    {
        return (m_buff.size() == 0);
    }
    bool is_valid() const
    {
        return (m_data.path != nullptr);
    }
    const url_data& data() const
    {
        return m_data;
    }
    const char* c_str() const
    {
        return m_data.str[gkr_url_part_whole];
    }
    std::size_t length() const
    {
        return m_data.len[gkr_url_part_whole];
    }

public:
    void clear() noexcept
    {
        m_buff.change_size(0);
        m_data = url_data{};
    }
    bool assign(const char* str, std::size_t len = std::size_t(-1), bool unescape = false)
    {
        Check_Arg_NotNull(str, false);

        if(len == std::size_t(-1)) len = std::strlen(str);

        m_buff.resize(len + 1);

        std::memcpy(m_buff.data(), str, len);

        char* buf = m_buff.data<char>();

        buf[len] = 0;

        return gkr_i2b(gkr_url_decompose(&m_data, buf, unsigned(len)));
    }

public:
    bool change_scheme(const char* str, std::size_t len = std::size_t(-1))
    {
        return change_part(gkr_url_part_scheme, str, len);
    }
    bool change_host(const char* str, std::size_t len = std::size_t(-1))
    {
        return change_part(gkr_url_part_host, str, len);
    }
    bool change_port(const char* str, std::size_t len = std::size_t(-1))
    {
        return change_part(gkr_url_part_host, str, len);
    }

public:
    bool change_path(const char* str, std::size_t len = std::size_t(-1))
    {
        return change_part(gkr_url_part_path, str, len);
    }
    bool change_query(const char* str, std::size_t len = std::size_t(-1))
    {
        return change_part(gkr_url_part_query, str, len);
    }
    bool change_fragment(const char* str, std::size_t len = std::size_t(-1))
    {
        return change_part(gkr_url_part_fragment, str, len);
    }

public:
    bool change_username(const char* str, std::size_t len = std::size_t(-1))
    {
        return change_part(gkr_url_part_username, str, len);
    }
    bool change_password(const char* str, std::size_t len = std::size_t(-1))
    {
        return change_part(gkr_url_part_password, str, len);
    }

private:
    bool change_part(url_part part, const char* str, std::size_t len)
    {
        if(str == nullptr) len = 0; else if(len == std::size_t(-1)) len = std::strlen(str);

        const std::size_t cch = m_buff.size() + len - m_data.len[gkr_url_part_scheme];

        m_buff.resize(cch);

        return gkr_i2b(gkr_url_change_part(part, &m_data, m_buff.data<char>(), unsigned(cch), str, unsigned(len)));
    }

};

using url = basic_url<std::allocator<char>>;
}
