#pragma once

#include <gkr/capi/params.h>

#include <gkr/sys/rw_lock.hpp>
#include <gkr/misc/fake_shared_mutex.hpp>
#include <gkr/container/raw_buffer.hpp>

#include <shared_mutex>

namespace gkr
{

struct node_t;
struct text_t;

class params
{
    params(      params&&) noexcept = delete;
    params(const params& ) noexcept = delete;

    params& operator=(      params&&) noexcept = delete;
    params& operator=(const params& ) noexcept = delete;

    friend static void lifecycle(params*, void*);

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
    node_t*     m_nodes  = nullptr;
    char*       m_texts  = nullptr;
    void*       m_param  = nullptr;

    std::size_t m_pitch  = DEFAULT_PITCH;
    std::size_t m_count  = 0;
    std::size_t m_offset = 0;

protected:
    GKR_CORE_API params(std::size_t pitch = DEFAULT_PITCH) noexcept;

public:
    GKR_CORE_API virtual ~params();

public:
    std::size_t get_nodes_count() const noexcept
    {
        return m_count;
    }

public:
    GKR_CORE_API bool get_info(std::size_t root, std::size_t& count, std::size_t& size, std::size_t& depth) const;

public:
    GKR_CORE_API bool copy_params(const params& other, std::size_t root = 0, std::size_t pitch = 0);

    GKR_CORE_API void reserve(std::size_t size, std::size_t pitch = 0);
    GKR_CORE_API void compact();

public:
    GKR_CORE_API virtual void clear();

protected:
    virtual char* realloc(std::size_t new_memory_footprint) = 0;

public:
    virtual std::size_t get_memory_footprint() const = 0;

public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual bool try_lock() = 0;

    virtual void lock_shared() const = 0;
    virtual void unlock_shared() const = 0;
    virtual bool try_lock_shared() const = 0;

public:
    std::unique_lock<params> get_writer_lock()
    {
        return std::unique_lock<params>(*this);
    }
    std::shared_lock<const params> get_reader_lock() const
    {
        return std::shared_lock<const params>(*this);
    }

public:
    GKR_CORE_API std::size_t add_object(const char* key, std::size_t root = 0);
    GKR_CORE_API std::size_t add_array (const char* key, std::size_t root = 0);
    GKR_CORE_API std::size_t add_null  (const char* key, std::size_t root = 0);

public:
    GKR_CORE_API std::size_t set_value(const char* key, bool        value, std::size_t root = 0, bool overwrite = false);
    GKR_CORE_API std::size_t set_value(const char* key, double      value, std::size_t root = 0, bool overwrite = false);
    GKR_CORE_API std::size_t set_value(const char* key, long long   value, std::size_t root = 0, bool overwrite = false);
    GKR_CORE_API std::size_t set_value(const char* key, const char* value, std::size_t root = 0, bool overwrite = false);

public:
    std::size_t set_value(const char* key, float value, std::size_t root = 0, bool overwrite = false)
    {
        return set_value(key, double(value), root, overwrite);
    }
    std::size_t set_value(const char* key, long double value, std::size_t root = 0, bool overwrite = false)
    {
        return set_value(key, double(value), root, overwrite);
    }
    template<typename T>
    std::size_t set_value(const char* key, T value, std::size_t root = 0, bool overwrite = false)
    {
        static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "Type is not convertible to param value");

        return set_value(key, static_cast<long long>(value), root, overwrite);
    }

public:
    GKR_CORE_API std::size_t find_node(const char* key, std::size_t root = 0) const;

    GKR_CORE_API param_type_t get_type(std::size_t index) const;

    GKR_CORE_API bool        get_value(std::size_t index, bool        def_val = false  ) const;
    GKR_CORE_API double      get_value(std::size_t index, double      def_val = 0.0    ) const;
    GKR_CORE_API long long   get_value(std::size_t index, long long   def_val = 0      ) const;
    GKR_CORE_API const char* get_value(std::size_t index, const char* def_val = nullptr) const;

public:
    bool contains_value(const char* key, std::size_t root = 0) const
    {
        return (get_type(find_node(key, root)) != param_type_none);
    }
    param_type_t get_type(const char* key, std::size_t root = 0) const
    {
        return get_type(find_node(key, root));
    }

public:
    inline bool get_value(const char* key, std::size_t root, bool def_val) const
    {
        return get_value(find_node(key, root), def_val);
    }

    inline const char* get_value(const char* key, std::size_t root, decltype(nullptr)) const
    {
        return get_value(find_node(key, root), static_cast<const char*>(nullptr));
    }
    inline const char* get_value(const char* key, std::size_t root, const char* def_val) const
    {
        return get_value(find_node(key, root), def_val);
    }

    inline float get_value(const char* key, std::size_t root, float def_val) const
    {
        return float(get_value(find_node(key, root), double(def_val)));
    }
    inline double get_value(const char* key, std::size_t root, double def_val) const
    {
        return double(get_value(find_node(key, root), double(def_val)));
    }
    inline long double get_value(const char* key, std::size_t root, long double def_val) const
    {
        return static_cast<long double>(get_value(find_node(key, root), double(def_val)));
    }

    template<typename T>
    inline T get_value(const char* key, std::size_t root, T def_val) const
    {
        static_assert(std::is_integral<T>::value || std::is_enum<T>::value, "Type is not convertible to param value");

        return static_cast<T>(get_value(find_node(key, root), static_cast<long long>(def_val)));
    }

private:
    const
    node_t& get_node(std::size_t index) const;
    node_t& get_node(std::size_t index);

    const
    text_t& get_text(std::size_t offset) const;
    text_t& get_text(std::size_t offset);

private:
    bool keys_are_equal(const char* key, std::size_t len, std::size_t  ofs) const;
    bool peek_array_pos(const char* key, std::size_t len, std::size_t& pos) const noexcept;

private:
    std::size_t insert_text(std::size_t index, const char* ptr, std::size_t len);
    std::size_t insert_node();

    std::size_t create_node(const char* key, std::size_t len, std::size_t prev, std::size_t parent, const char* sep);

    std::size_t append_node(const char* key, std::size_t root);
    std::size_t lookup_node(const char* key, std::size_t parent);

    std::size_t mirror_node(bool has_name, const params& other, std::size_t other_parent, std::size_t self_parent);

private:
    std::size_t search_node(const char* key, std::size_t parent) const;

private:
    void collect_info(bool has_name, std::size_t index, std::size_t level, std::size_t& count, std::size_t& size, std::size_t& depth) const;
};

template<class RecursiveSharedMutex, class Allocator=std::allocator<std::max_align_t>>
class basic_params final : public params
{
    basic_params(const basic_params& ) noexcept = delete;
    basic_params(      basic_params&&) noexcept = delete;

    basic_params& operator=(const basic_params& ) noexcept = delete;
    basic_params& operator=(      basic_params&&) noexcept = delete;

    using selt_t   = basic_params<RecursiveSharedMutex, Allocator>;
    using mutex_t  = RecursiveSharedMutex;
    using buffer_t = basic_raw_buffer<Allocator>;

private:
    mutable
    mutex_t   m_mutex;
    buffer_t  m_buffer;

public:
    basic_params(const Allocator& allocator = Allocator()) noexcept(
        std::is_nothrow_constructible<buffer_t, Allocator>::value
        )
        : params()
        , m_buffer(allocator)
    {
    }
    basic_params(std::size_t pitch, const Allocator& allocator = Allocator()) noexcept(
        std::is_nothrow_constructible<buffer_t, Allocator>::value
        )
        : params(pitch)
        , m_buffer(allocator)
    {
    }
    virtual ~basic_params() noexcept override
    {
    }

public:
    virtual void clear() 
    {
        std::unique_lock<selt_t> lock(*this);
        params ::clear();
        m_buffer.clear();
    }

protected:
    virtual char* realloc(std::size_t new_memory_footprint)
    {
        m_buffer.resize(new_memory_footprint);
        return m_buffer.data<char>();
    }

public:
    virtual std::size_t get_memory_footprint() const noexcept
    {
        return m_buffer.size();
    }

public:
    virtual void lock() override
    {
        m_mutex.lock();
    }
    virtual void unlock() override
    {
        m_mutex.unlock();
    }
    virtual bool try_lock() override
    {
        return m_mutex.try_lock();
    }
    virtual void lock_shared() const override
    {
        m_mutex.lock_shared();
    }
    virtual void unlock_shared() const override
    {
        m_mutex.unlock_shared();
    }
    virtual bool try_lock_shared() const override
    {
        return m_mutex.try_lock_shared();
    }
};

template<typename Allocator=std::allocator<std::max_align_t>> using basic_singlethreaded_params = basic_params<misc::fake_shared_mutex, Allocator>;
template<typename Allocator=std::allocator<std::max_align_t>> using  basic_multithreaded_params = basic_params< sys::recursive_rw_lock, Allocator>;

using singlethreaded_params = basic_params<misc::fake_shared_mutex, std::allocator<std::max_align_t>>;
using  multithreaded_params = basic_params< sys::recursive_rw_lock, std::allocator<std::max_align_t>>;

}
