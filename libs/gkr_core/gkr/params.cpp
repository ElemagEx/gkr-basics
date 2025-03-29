#include <gkr/defs.hpp>

#ifndef GKR_DONT_HIDE_CDEFS
#define GKR_DONT_HIDE_CDEFS
#endif
#include <gkr/params.hpp>

#include <gkr/diagnostics.hpp>

#include <cstring>
#include <cctype>

extern "C"
{
struct gkr_params* gkr_params_create()
{
    return reinterpret_cast<struct gkr_params*>(new gkr::params());
}

int gkr_params_destroy(struct gkr_params* params)
{
    Check_Arg_NotNull(params, 0);
    delete reinterpret_cast<gkr::params*>(params);
    return 1;
}
int gkr_params_clear(struct gkr_params* params)
{
    Check_Arg_NotNull(params, 0);
    reinterpret_cast<gkr::params*>(params)->clear();
    return 1;
}
int gkr_params_reserve(struct gkr_params* params, size_t size)
{
    Check_Arg_NotNull(params, 0);
    reinterpret_cast<gkr::params*>(params)->reserve(size);
    return 1;
}
size_t gkr_params_insert_null_value(struct gkr_params* params, const char* key)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_value(key, nullptr);
}
size_t gkr_params_insert_real_value(struct gkr_params* params, const char* key, double value)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_value(key, value);
}
size_t gkr_params_insert_integer_value(struct gkr_params* params, const char* key, long long value)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_value(key, value);
}
size_t gkr_params_insert_string_value(struct gkr_params* params, const char* key, const char* value)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_value(key, value);
}
}


namespace gkr
{

template<std::size_t Alignment>
std::size_t align(std::size_t size)
{
    static_assert((Alignment & (Alignment - 1)) == 0, "Alignment must be power of 2");

    if((size &   (Alignment - 1)) != 0)
    {
        size &= ~(Alignment - 1);
        size +=  (Alignment);
    }
    return size;
}

using type_t   = params::param_type_t;
using index_t  = unsigned short;
using key_t    = unsigned int;
using value_t  = union
{
    unsigned
    long long   zero;
    double      real;
    long long   integer;
    bool        boolean;
    std::size_t string;
    index_t     child;
};
struct head_t
{
    GKR_WARNING_DISABLE(4200, abcd)
    index_t     ref;
    char        str[0];
    GKR_WARNING_DEFAULT(4200, abcd)
};
struct node_t
{
    value_t     value;
    key_t       key;
    index_t     next;
    type_t      type;
};
static_assert( sizeof(struct node_t) == 16, "Keep it 16 bytes");
static_assert(alignof(struct node_t) ==  8, "Keep it  8 bytes");

constexpr key_t BAD_KEY = key_t(-1);

inline type_t determine_node_type(const char* sep)
{
    if((sep == nullptr) || (sep[1] == 0)) return params::param_type_none;

    return std::isdigit(sep[1])
        ? params::param_type_array
        : params::param_type_object;
}
inline std::size_t array_pos_as_key(std::size_t pos)
{
    return (pos << 1) | 1;
}
inline std::size_t key_as_array_pos(std::size_t key)
{
    return (key >> 1);
}
inline bool is_array_pos_key(std::size_t key)
{
    return ((key & 1) != 0);
}
inline bool is_obj_name_key(std::size_t key)
{
    return ((key & 1) == 0);
}

params::params() noexcept(std::is_nothrow_default_constructible<buffer_t>::value)
{
    static_assert(alignof(struct node_t) <= alignof(buffer_t::allocator_traits::value_type), "Must be that way");
}

params::~params() noexcept(std::is_nothrow_destructible<buffer_t>::value)
{
}

void params::clear() noexcept
{
    m_buff.clear();
    m_offset = 0U;
    m_nodes  = 0U;
    m_root   = 0U;
    m_type   = param_type_none;
}

void params::reserve(std::size_t size)
{
    size = align<GRANULARITY>(size);

    const std::size_t old_capacity = m_buff.capacity();
    const std::size_t new_capacity = size;

    if(new_capacity <= old_capacity) return;

    const std::size_t size_to_move = m_nodes * sizeof(struct node_t);

    m_buff.reserve(new_capacity);
    m_buff.change_size(size);

    std::memmove(
        m_buff.data<char>() + new_capacity - size_to_move,
        m_buff.data<char>() + old_capacity - size_to_move,
        size_to_move);
}

std::size_t params::insert_value(const char* key, decltype(nullptr))
{
    const std::size_t index = lookup_node(key, m_root, 0);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_null: break;
        case param_type_none: node.type = param_type_null; break;
        default: return 0;
    }
    return index;
}

std::size_t params::insert_value(const char* key, bool value)
{
    const std::size_t index = lookup_node(key, m_root, 0);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_boolean: break;
        case param_type_none   : node.type = param_type_boolean; break;
        default: return 0;
    }
    node.value.boolean = value;
    return index;
}

std::size_t params::insert_value(const char* key, double value)
{
    const std::size_t index = lookup_node(key, m_root, 0);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_double: break;
        case param_type_none  : node.type = param_type_double; break;
        default: return 0;
    }
    node.value.real = value;
    return index;
}

std::size_t params::insert_value(const char* key, long long value)
{
    const std::size_t index = lookup_node(key, m_root, 0);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_integer: break;
        case param_type_none   : node.type = param_type_integer; break;
        default: return 0;
    }
    node.value.integer = value;
    return index;
}

std::size_t params::insert_value(const char* key, const char* value)
{
    Check_Arg_NotNull(value, 0);

    const std::size_t index = lookup_node(key, m_root, 0);

    if(index == 0) return 0;

    switch(int(get_node(index).type))
    {
        case param_type_none  :
        case param_type_string:
            break;

        default: return 0;
    }
    const std::size_t offset = insert_text(index, value, std::strlen(value));

    node_t& node = get_node(index);

    node.type         = param_type_string;
    node.value.string = offset;
    return index;
}

std::size_t params::find_value(const char* key) const noexcept(DIAG_NOEXCEPT)
{
    const std::size_t index = search_node(key, m_root);

    return index;
}

type_t params::get_type(std::size_t index) const noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid(index <= m_nodes, param_type_none);

    if(index == 0) return param_type_none;

    const node_t& node = get_node(index);

    return node.type;
}

type_t params::get_container(std::size_t index) const noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid(index <= m_nodes, param_type_none);

    if(index == 0) return param_type_none;

    const node_t& node = get_node(index);

    if(node.key == key_t(BAD_KEY)) return param_type_null;
    if(is_array_pos_key(node.key)) return param_type_array;
    if(is_obj_name_key (node.key)) return param_type_object;

    Check_Recovery("Something is wrong");

    return param_type_none;
}

bool params::get_value(std::size_t index, bool def_val) const noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid(index <= m_nodes, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_boolean) return def_val;

    return node.value.boolean;
}

double params::get_value(std::size_t index, double def_val) const noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid(index <= m_nodes, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_double) return def_val;

    return node.value.real;
}

long long params::get_value(std::size_t index, long long def_val) const noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid(index <= m_nodes, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_integer) return def_val;

    return node.value.integer;
}

const char* params::get_value(std::size_t index, const char* def_val) const noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid(index <= m_nodes, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_string) return def_val;

    const struct head_t& head = m_buff.as<struct head_t>(node.value.string);

    return head.str;
}

const struct node_t& params::get_node(std::size_t index) const noexcept(DIAG_NOEXCEPT)
{
    return *(m_buff.data<struct node_t>(m_buff.capacity()) - index);
}

struct node_t& params::get_node(std::size_t index) noexcept(DIAG_NOEXCEPT)
{
    return *(m_buff.data<struct node_t>(m_buff.capacity()) - index);
}

bool params::keys_are_equal(const char* key, std::size_t len, std::size_t ofs) const noexcept(DIAG_NOEXCEPT)
{
    const struct head_t& head = m_buff.as<struct head_t>(ofs);

    return !std::memcmp(head.str, key, len) && (head.str[len] == 0);
}

bool params::peek_array_pos(const char* key, std::size_t len, std::size_t& pos) const noexcept
{
    if(len > 9) return false;

    for(pos = 0; std::isdigit(*key); ++key, --len)
    {
        pos = (pos * 10) + (*key - '0');
    }
    return (len == 0);
}

std::size_t params::insert_text(std::size_t index, const char* text, std::size_t len)
{
    const std::size_t need_size = align<sizeof(struct head_t)>(sizeof(struct head_t) + len + 1);

    const std::size_t busy_size = m_nodes * sizeof(struct node_t) + m_offset;
    const std::size_t free_size = m_buff.size() - busy_size;

    if(need_size > free_size)
    {
        reserve(need_size);
    }
    const std::size_t offset = m_offset;

    m_offset += need_size;

    struct head_t& head = m_buff.as<struct head_t>(offset);

    head.ref      = index_t(index);
    head.str[len] = 0;

    std::memcpy(head.str, text, len);

    return offset;
}

std::size_t params::insert_node()
{
    if(m_nodes >= 0x10000) return 0;

    const std::size_t need_size = sizeof(struct node_t);

    const std::size_t busy_size = m_nodes * sizeof(struct node_t) + m_offset;
    const std::size_t free_size = m_buff.size() - busy_size;

    if(need_size > free_size)
    {
        reserve(need_size);
    }
    return ++m_nodes;
}

std::size_t params::create_node(std::size_t offset, std::size_t prev, std::size_t parent, const char* sep)
{
    const std::size_t index = insert_node();
    if(index == 0) return 0;

    if(prev   != 0) get_node(prev  ).next        = index_t(index);
    if(parent != 0) get_node(parent).value.child = index_t(index);

    node_t& node = get_node(index);
    node.value.zero = 0;
    node.key  = key_t(offset);
    node.next = 0;
    node.type = determine_node_type(sep);

    return index;
}

std::size_t params::lookup_node(const char* key, std::size_t child, std::size_t parent)
{
    std::size_t index = child;

    if(key == nullptr) // add value to root
    {
        return (index != 0)
            ? index
            : create_node(BAD_KEY, 0, 0, nullptr);
    }

    const char* sep = std::strchr(key, '/');
    const bool last = (sep == nullptr);

    const std::size_t len = !last
        ? (sep - key)
        : std::strlen(key)
        ;
    if(len == 0) return 0;

    if(std::isdigit(*key)) // add value to array
    {
        std::size_t pos;
        if(!peek_array_pos(key, len, pos)) return 0;

        for(std::size_t prev = 0; ; )
        {
            if(index == 0)
            {
                index = create_node(array_pos_as_key(pos), prev, parent, sep);
                child = 0;
                break;
            }
            node_t& node = get_node(index);

            if(!is_array_pos_key(node.key)) return 0;

            if(key_as_array_pos(node.key) == pos)
            {
                child = node.value.child;
                break;
            }
            prev  = index;
            index = get_node(index).next;
        }
    }
    else // add value to object
    {
        for(std::size_t prev = 0; ; )
        {
            if(index == 0)
            {
                index = create_node(insert_text(index, key, len), prev, parent, sep);
                child = 0;
                break;
            }
            node_t& node = get_node(index);

            if(!is_obj_name_key(node.key)) return 0;

            if(keys_are_equal(key, len, node.key))
            {
                child = node.value.child;
                break;
            }
            prev  = index;
            index = get_node(index).next;
        }
    }
    if(last) return index;

    return lookup_node(sep + 1, child, index);
}

std::size_t params::search_node(const char* key, std::size_t child) const noexcept(DIAG_NOEXCEPT)
{
    std::size_t index = child;

    if(key == nullptr) // search in root
    {
        return index;
    }

    const char* sep = std::strchr(key, '/');
    const bool last = (sep == nullptr);

    const std::size_t len = !last
        ? (sep - key)
        : std::strlen(key)
        ;
    if(len == 0) return 0;

    if(std::isdigit(*key)) // search in array
    {
        std::size_t pos;
        if(!peek_array_pos(key, len, pos)) return 0;

        for( ; ; )
        {
            if(index == 0) return 0;

            const node_t& node = get_node(index);

            if(!is_array_pos_key(node.key)) return 0;

            if(key_as_array_pos(node.key) == pos)
            {
                child = node.value.child;
                break;
            }
            index = get_node(index).next;
        }
    }
    else // search in object
    {
        for( ; ; )
        {
            if(index == 0) return 0;

            const node_t& node = get_node(index);

            if(!is_obj_name_key(node.key)) return 0;

            if(keys_are_equal(key, len, node.key))
            {
                child = node.value.child;
                break;
            }
            index = get_node(index).next;
        }
    }
    if(last) return index;

    return search_node(sep + 1, child);
}

}
