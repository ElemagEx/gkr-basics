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

    basic_url           (const basic_url&  other) noexcept(std::is_nothrow_copy_constructible<buff_t>::value) : m_buff(  other.m_buff), m_parts(  other.m_parts) {}
    basic_url& operator=(const basic_url&  other) noexcept(std::is_nothrow_copy_assignable   <buff_t>::value) { m_buff = other.m_buff;  m_parts = other.m_parts; return *this; }

    basic_url           (basic_url&& other) noexcept(std::is_nothrow_move_constructible<buff_t>::value) : m_buff(  std::move(other.m_buff)), m_parts(  std::exchange(other.m_parts, {})) {}
    basic_url& operator=(basic_url&& other) noexcept(std::is_nothrow_move_assignable   <buff_t>::value) { m_buff = std::move(other.m_buff);  m_parts = std::exchange(other.m_parts, {}); return *this; }

public:
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
    bool decompose(const char* str, bool unescape = false)
    {
        Check_Arg_NotNull(str, false);
        const std::size_t size = std::strlen(str) + 1;

        m_buff.resize(size + 2);
        std::memcpy(m_buff.data(), str, size);

        return (0 != gkr_url_decompose(m_buff.template data<char>(), gkr_b2i(unescape), &m_parts));
    }

public:
    bool construct(const url_parts& parts, bool unescape = false)
    {
        const int cch = gkr_url_construct(&parts, nullptr, 0);
        Check_Sys_Result(cch, false);

        buff_t buff(cch);
        const int len = gkr_url_construct(&parts, buff.template data<char>(), cch);
        Check_Sys_Result(len, false);
        buff.change_size(len + 1);

        m_buff = std::move(buff);

        return (0 <= gkr_url_decompose(m_buff.template data<char>(), gkr_b2i(unescape), &m_parts));
    }
};

using url = basic_url<std::allocator<char>>;
}
