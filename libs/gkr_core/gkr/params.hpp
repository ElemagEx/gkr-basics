#pragma once

#include <gkr/capi/params.h>

#include <gkr/container/raw_buffer.hpp>

namespace gkr
{

class params
{
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

    static constexpr std::size_t MINIMUM_PITCH =   16;
    static constexpr std::size_t DEFAULT_PITCH = 1024;

private:
    using buffer_t = raw_buffer;

    buffer_t    m_buff;
    std::size_t m_pitch  = DEFAULT_PITCH;
    std::size_t m_offset = 0;
    std::size_t m_nodes  = 0;
    std::size_t m_root   = 0;

    template<typename Functor> bool traverse(bool skip_siblings, std::size_t index, Functor&& functor) const;

public:
    GKR_CORE_API  params() noexcept(std::is_nothrow_default_constructible<buffer_t>::value);
    GKR_CORE_API ~params() noexcept(std::is_nothrow_destructible         <buffer_t>::value);

    params(std::size_t size, std::size_t pitch = DEFAULT_PITCH)
    {
        reserve(size, pitch);
    }
    params(params&& other) noexcept(
        std::is_nothrow_move_constructible<buffer_t>::value
        )
        : m_buff  (std::move    (other.m_buff))
        , m_pitch (std::exchange(other.m_pitch , DEFAULT_PITCH))
        , m_offset(std::exchange(other.m_offset, 0U))
        , m_nodes (std::exchange(other.m_nodes , 0U))
        , m_root  (std::exchange(other.m_root  , 0U))
    {
    }
    params& operator=(params&& other) noexcept(
        std::is_nothrow_move_assignable<buffer_t>::value
        )
    {
        if(this != &other)
        {
            m_buff   = std::move    (other.m_buff);
            m_pitch  = std::exchange(other.m_pitch , DEFAULT_PITCH);
            m_offset = std::exchange(other.m_offset, 0U);
            m_nodes  = std::exchange(other.m_nodes , 0U);
            m_root   = std::exchange(other.m_root  , 0U);
        }
        return *this;
    }

    params(const params& other)
        : m_buff  (buffer_t::allocator_traits::select_on_container_copy_construction(other.m_buff.get_allocator()))
        , m_pitch (other.m_pitch)
        , m_offset(0U)
        , m_nodes (0U)
        , m_root  (0U)
    {
        copy_params(other);
    }
    params& operator=(const params& other)
    {
        if(this != &other)
        {
            m_buff.on_copy_assignment(other.m_buff);
            m_pitch = other.m_pitch;
            copy_params(other);
        }
        return *this;
    }

public:
    GKR_CORE_API void copy_params(const params& other, std::size_t pitch = 0);

public:
    GKR_CORE_API void clear() noexcept;
    GKR_CORE_API void reserve(std::size_t size, std::size_t pitch = 0);

public:
    GKR_CORE_API params& reset_root(std::size_t index = 0);

public:
    GKR_CORE_API std::size_t insert_object(const char* key);
    GKR_CORE_API std::size_t insert_array (const char* key);
    GKR_CORE_API std::size_t insert_null  (const char* key);

public:
    GKR_CORE_API std::size_t insert_value(const char* key, bool value);
    GKR_CORE_API std::size_t insert_value(const char* key, double value);
    GKR_CORE_API std::size_t insert_value(const char* key, long long value);
    GKR_CORE_API std::size_t insert_value(const char* key, const char* value);

public:
    std::size_t insert_value(const char* key, float value)
    {
        return insert_value(key, double(value));
    }
    std::size_t insert_value(const char* key, long double value)
    {
        return insert_value(key, double(value));
    }
    template<typename T>
    std::size_t insert_value(const char* key, T value)
    {
        static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "Type is not convertible to param value");

        return insert_value(key, (long long)(value));
    }

public:
    GKR_CORE_API std::size_t find_value(const char* key) const;

    GKR_CORE_API param_type_t get_type(std::size_t index) const;

    GKR_CORE_API bool        get_value(std::size_t index, bool        def_val = false  ) const;
    GKR_CORE_API double      get_value(std::size_t index, double      def_val = 0.0    ) const;
    GKR_CORE_API long long   get_value(std::size_t index, long long   def_val = 0      ) const;
    GKR_CORE_API const char* get_value(std::size_t index, const char* def_val = nullptr) const;

public:
    bool contains_value(const char* key) const
    {
        return (get_type(find_value(key)) != param_type_none);
    }
    param_type_t get_type(const char* key) const
    {
        return get_type(find_value(key));
    }

public:
    template<typename T>
    T get_value(const char* key, T def_val) const
    {
        static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "Type is not convertible to param value");

        return T(get_value(find_value(key), (long long)(def_val)));
    }

    template<>
    const char* get_value<const char*>(const char* key, const char* def_val) const
    {
        return get_value(find_value(key), def_val);
    }
    template<>
    bool get_value<bool>(const char* key, bool def_val) const
    {
        return get_value(find_value(key), def_val);
    }

    template<>
    float get_value<float>(const char* key, float def_val) const
    {
        return float(get_value(find_value(key), double(def_val)));
    }
    template<>
    double get_value<double>(const char* key, double def_val) const
    {
        return double(get_value(find_value(key), double(def_val)));
    }
    template<>
    long double get_value<long double>(const char* key, long double def_val) const
    {
        return long double(get_value(find_value(key), double(def_val)));
    }

public:
    void get_info(std::size_t& count, std::size_t& size, std::size_t& depth) const;

private:
    const
    struct node_t& get_node(std::size_t index) const;
    struct node_t& get_node(std::size_t index);

    const char* get_text(std::size_t key) const;

private:
    bool keys_are_equal(const char* key, std::size_t len, std::size_t  ofs) const;
    bool peek_array_pos(const char* key, std::size_t len, std::size_t& pos) const noexcept;

private:
    std::size_t insert_text(std::size_t index, const char* text, std::size_t len);
    std::size_t insert_node();

    std::size_t create_node(const char* key, std::size_t len, std::size_t prev, std::size_t parent, const char* sep);

    std::size_t append_node(const char* key);
    std::size_t lookup_node(const char* key, std::size_t parent);

    std::size_t mirror_node(bool has_name, const params& other, std::size_t other_parent, std::size_t self_parent);

private:
    std::size_t search_node(const char* key, std::size_t parent) const;

private:
    void collect_info(bool has_name, std::size_t index, std::size_t level, std::size_t& count, std::size_t& size, std::size_t& depth) const;
};

}
