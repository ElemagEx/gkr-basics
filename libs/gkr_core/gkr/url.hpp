#pragma once

#include <gkr/capi/url.h>

#include <gkr/diagnostics.hpp>
#include <gkr/container/raw_buffer.hpp>

namespace gkr
{

using url_parts = gkr_url_parts;

template<typename Allocator>
class basic_url
{
    using buff_t = basic_raw_buffer<Allocator>;

    buff_t    m_buff;
    url_parts m_parts {};

public:
    basic_url() noexcept(std::is_nothrow_default_constructible<buff_t>::value) = default;
   ~basic_url() noexcept(std::is_nothrow_destructible         <buff_t>::value) = default;

    basic_url(basic_url&& other) noexcept(std::is_nothrow_move_constructible<buff_t>::value)
        : m_buff (std::move    (other.m_buff))
        , m_parts(std::exchange(other.m_parts, {}))
    {
    }
    basic_url(const basic_url& other) noexcept(std::is_nothrow_copy_constructible<buff_t>::value)
        : m_buff (other.m_buff)
        , m_parts(other.m_parts)
    {
    }
    basic_url& operator=(basic_url&& other) noexcept(std::is_nothrow_move_assignable<buff_t>::value)
    {
        m_buff  = std::move    (other.m_buff);
        m_parts = std::exchange(other.m_parts, {});
        return *this;
    }
    basic_url& operator=(const basic_url& other) noexcept(std::is_nothrow_copy_assignable<buff_t>::value)
    {
        m_buff  = other.m_buff;
        m_parts = other.m_parts;
        return *this;
    }

public:
    basic_url(std::size_t capacity) : m_buff(capacity)
    {
    }
    basic_url(const char* str, bool unescape = false)
    {
        decompose(str, unescape);
    }

public:
    bool is_empty() const
    {
        return (m_buff.size() == 0);
    }
    bool is_valid() const
    {
        return (m_parts.path != nullptr);
    }
    const url_parts& parts() const
    {
        return m_parts;
    }

public:
    void clear() noexcept
    {
        m_buff.change_size(0);
        m_parts = url_parts{};
    }
    bool decompose(const char* str, std::size_t len = std::size_t(-1), bool unescape = false)
    {
        Check_Arg_NotNull(str, false);

        if(len == std::size_t(-1)) len = std::strlen(str);

        m_buff.resize(len + 1);

        std::memcpy(m_buff.data(), str, len);

        m_buff.at<char>(len) = 0;

        if(!gkr_url_decompose(&m_parts, m_buff.data<char>(), unsigned(len)))
        {
            m_parts = url_parts {};
            return false;
        }
        return true;
    }

public:
    bool change_scheme(const char* scheme, std::size_t len = std::size_t(-1))
    {
        if(scheme == nullptr) len = 0; else if(len == std::size_t(-1)) len = std::strlen(scheme);

        const std::size_t cch = m_buff.size() + len - m_parts.len[gkr_url_part_scheme];

        m_buff.resize(cch);

        return gkr_i2b(gkr_url_change_part(gkr_url_part_scheme, &m_parts, m_buff.data<char>(), unsigned(cch), scheme, unsigned(len)));
    }

};

using url = basic_url<std::allocator<char>>;
}
