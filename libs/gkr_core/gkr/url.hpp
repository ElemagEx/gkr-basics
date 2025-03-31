#pragma once

#include <gkr/capi/url.h>

#include <gkr/diagnostics.hpp>
#include <gkr/container/raw_buffer.hpp>

#include <string>

namespace gkr
{

using url_parts = gkr_url_parts;

template<typename Allocator>
class basic_url
{
    using buff_t = basic_raw_buffer<Allocator>;

    buff_t    m_data;
    url_parts m_parts;

public:
    basic_url() noexcept(std::is_nothrow_default_constructible<buff_t>::value) = default;
   ~basic_url() noexcept(std::is_nothrow_destructible         <buff_t>::value) = default;

    basic_url           (const basic_url&  other) noexcept(std::is_nothrow_copy_constructible<buff_t>::value) : m_data(  other.m_data), m_parts(  other.m_parts) {}
    basic_url& operator=(const basic_url&  other) noexcept(std::is_nothrow_copy_assignable   <buff_t>::value) { m_data = other.m_data;  m_parts = other.m_parts; return *this; }

    basic_url           (basic_url&& other) noexcept(std::is_nothrow_move_constructible<buff_t>::value) : m_data(  std::move(other.m_data)), m_parts(  std::exchange(other.m_parts, {})) {}
    basic_url& operator=(basic_url&& other) noexcept(std::is_nothrow_move_assignable   <buff_t>::value) { m_data = std::move(other.m_data);  m_parts = std::exchange(other.m_parts, {}); return *this; }

public:
    basic_url(const char* str, bool unescape = false)
    {
        reset(str, unescape);
    }
    basic_url(const std::string& str, bool unescape = false)
    {
        reset(str, unescape);
    }

public:
    bool is_empty() const
    {
        return (m_data.size() == 0);
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
    void reset() noexcept
    {
        m_data.change_size(0);
        m_parts = url_parts{};
    }
    void reset(const char* str, bool unescape = false)
    {
        Check_Arg_NotNull(str, );
        const std::size_t size = std::strlen(str) + 1;

        m_data.resize(size);
        std::memcpy(m_data.data(), str, size);

        gkr_url_decompose(m_data.template data<char>(), gkr_b2i(unescape), &m_parts);
    }
    void reset(const std::string& str, bool unescape = false)
    {
        const std::size_t size = str.size() + 1;

        m_data.resize(size);
        std::memcpy(m_data.data(), str.c_str(), size);

        gkr_url_decompose(m_data.template data<char>(), gkr_b2i(unescape), &m_parts);
    }
};

using url = basic_url<std::allocator<char>>;
}
