#pragma once

#ifndef GKR_DONT_HIDE_CDEFS
#define GKR_HIDE_PARAMS_CDEFS
#endif
#include <gkr/capi/params.h>

#include <gkr/container/raw_buffer.hpp>

#include <type_traits>

namespace gkr
{

class params
{
    params           (const params&) noexcept = delete;
    params& operator=(const params&) noexcept = delete;

public:
    using param_type_t = enum : unsigned short
    {
        param_type_none    = gkr_param_type_none   ,
        param_type_null    = gkr_param_type_null   ,
        param_type_array   = gkr_param_type_array  ,
        param_type_object  = gkr_param_type_object ,
        param_type_string  = gkr_param_type_string ,
        param_type_double  = gkr_param_type_double ,
        param_type_integer = gkr_param_type_integer,
        param_type_boolean = gkr_param_type_boolean,
    };

    static constexpr std::size_t GRANULARITY = 1024;

private:
    static_assert((GRANULARITY & (GRANULARITY - 1)) == 0, "Keep GRANULARITY as exact power of 2");

    using buffer_t = raw_buffer;

    buffer_t     m_buff;
    std::size_t  m_offset = 0;
    std::size_t  m_nodes  = 0;
    std::size_t  m_root   = 0;
    param_type_t m_type   = param_type_none;

public:
    GKR_CORE_API  params() noexcept(std::is_nothrow_default_constructible<buffer_t>::value);
    GKR_CORE_API ~params() noexcept(std::is_nothrow_destructible         <buffer_t>::value);

    params(std::size_t size)
    {
        reserve(size);
    }
    params(params&& other) noexcept(
        std::is_nothrow_move_constructible<buffer_t>::value
        )
        : m_buff  (std::move    (other.m_buff))
        , m_offset(std::exchange(other.m_offset, 0U))
        , m_nodes (std::exchange(other.m_nodes , 0U))
        , m_root  (std::exchange(other.m_root  , 0U))
        , m_type  (std::exchange(other.m_type  , param_type_none))
    {
    }
    params& operator=(params&& other) noexcept(
        std::is_nothrow_move_assignable<buffer_t>::value
        )
    {
        m_buff   = std::move    (other.m_buff);
        m_offset = std::exchange(other.m_offset, 0U);
        m_nodes  = std::exchange(other.m_nodes , 0U);
        m_root   = std::exchange(other.m_root  , 0U);
        m_type   = std::exchange(other.m_type  , param_type_none);
        return *this;
    }

public:
    GKR_CORE_API void clear() noexcept;
    GKR_CORE_API void reserve(std::size_t size);

public:
    GKR_CORE_API std::size_t insert_value(const char* key, decltype(nullptr));
    GKR_CORE_API std::size_t insert_value(const char* key, bool value);
    GKR_CORE_API std::size_t insert_value(const char* key, double value);
    GKR_CORE_API std::size_t insert_value(const char* key, long long value);
    GKR_CORE_API std::size_t insert_value(const char* key, const char* value);

public:
    std::size_t insert_value(const char* key, float value)
    {
        return insert_value(key, double(value));
    }
    std::size_t insert_value(const char* key, int value)
    {
        return insert_value(key, long long(value));
    }

public:
    GKR_CORE_API std::size_t find_value(const char* key) const noexcept(DIAG_NOEXCEPT);

    GKR_CORE_API param_type_t get_type     (std::size_t index) const noexcept(DIAG_NOEXCEPT);
    GKR_CORE_API param_type_t get_container(std::size_t index) const noexcept(DIAG_NOEXCEPT);

    GKR_CORE_API bool        get_value(std::size_t index, bool        def_val = false  ) const noexcept(DIAG_NOEXCEPT);
    GKR_CORE_API double      get_value(std::size_t index, double      def_val = 0.0    ) const noexcept(DIAG_NOEXCEPT);
    GKR_CORE_API long long   get_value(std::size_t index, long long   def_val = 0      ) const noexcept(DIAG_NOEXCEPT);
    GKR_CORE_API const char* get_value(std::size_t index, const char* def_val = nullptr) const noexcept(DIAG_NOEXCEPT);

public:
    bool contains_value(const char* key) const noexcept(DIAG_NOEXCEPT)
    {
        return (get_type(find_value(key)) != param_type_none);
    }

    param_type_t get_type(const char* key) const noexcept(DIAG_NOEXCEPT)
    {
        return get_type(find_value(key));
    }
    param_type_t get_container(const char* key) const noexcept(DIAG_NOEXCEPT)
    {
        return get_container(find_value(key));
    }

public:
    template<typename T>
    T get_value(const char* key, T def_val) const noexcept(DIAG_NOEXCEPT)
    {
        static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "Type not convertible to param value");

        return T(get_value(find_value(key), long long(def_val)));
    }

    template<>
    const char* get_value<const char*>(const char* key, const char* def_val) const noexcept(DIAG_NOEXCEPT)
    {
        return get_value(find_value(key), def_val);
    }
    template<>
    bool get_value<bool>(const char* key, bool def_val) const noexcept(DIAG_NOEXCEPT)
    {
        return get_value(find_value(key), def_val);
    }

    template<>
    float get_value<float>(const char* key, float def_val) const noexcept(DIAG_NOEXCEPT)
    {
        return float(get_value(find_value(key), double(def_val)));
    }
    template<>
    double get_value<double>(const char* key, double def_val) const noexcept(DIAG_NOEXCEPT)
    {
        return double(get_value(find_value(key), double(def_val)));
    }
    template<>
    long double get_value<long double>(const char* key, long double def_val) const noexcept(DIAG_NOEXCEPT)
    {
        return long double(get_value(find_value(key), double(def_val)));
    }

private:
    const
    struct node_t& get_node(std::size_t index) const noexcept(DIAG_NOEXCEPT);
    struct node_t& get_node(std::size_t index)       noexcept(DIAG_NOEXCEPT);

private:
    bool keys_are_equal(const char* key, std::size_t len, std::size_t  ofs) const noexcept(DIAG_NOEXCEPT);
    bool peek_array_pos(const char* key, std::size_t len, std::size_t& pos) const noexcept;

private:
    std::size_t insert_text(std::size_t index, const char* text, std::size_t len);
    std::size_t insert_node();

    std::size_t create_node(std::size_t offset, std::size_t prev, std::size_t parent, const char*);

    std::size_t lookup_node(const char* key, std::size_t child, std::size_t parent);

private:
    std::size_t search_node(const char* key, std::size_t child) const noexcept(DIAG_NOEXCEPT);
};

}
