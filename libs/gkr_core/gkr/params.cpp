#include <gkr/defs.hpp>

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
    Check_Arg_NotNull(params, false);
    delete reinterpret_cast<gkr::params*>(params);
    return true;
}
int gkr_params_copy(struct gkr_params* params, struct gkr_params* other_params, size_t pitch)
{
    Check_Arg_NotNull(params, false);
    Check_Arg_NotNull(other_params, false);
    reinterpret_cast<gkr::params*>(params)->copy_params(*reinterpret_cast<gkr::params*>(other_params), pitch);
    return true;
}
int gkr_params_clear(struct gkr_params* params)
{
    Check_Arg_NotNull(params, false);
    reinterpret_cast<gkr::params*>(params)->clear();
    return true;
}
int gkr_params_reserve(struct gkr_params* params, size_t size, size_t pitch)
{
    Check_Arg_NotNull(params, false);
    reinterpret_cast<gkr::params*>(params)->reserve(size, pitch);
    return true;
}
int gkr_params_reset_root(struct gkr_params* params, size_t index)
{
    Check_Arg_NotNull(params, false);
    reinterpret_cast<gkr::params*>(params)->reset_root(index);
    return true;
}
size_t gkr_params_insert_object(struct gkr_params* params, const char* key)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_object(key);
}
size_t gkr_params_insert_array(struct gkr_params* params, const char* key)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_array(key);
}
size_t gkr_params_insert_null(struct gkr_params* params, const char* key)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_null(key);
}
size_t gkr_params_insert_string_value(struct gkr_params* params, const char* key, const char* value)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_value(key, value);
}
size_t gkr_params_insert_double_value(struct gkr_params* params, const char* key, double value)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_value(key, value);
}
size_t gkr_params_insert_integer_value(struct gkr_params* params, const char* key, long long value)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_value(key, value);
}
size_t gkr_params_insert_boolean_value(struct gkr_params* params, const char* key, int value)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->insert_value(key, gkr_i2b(value));
}
size_t gkr_params_find_value(struct gkr_params* params, const char* key)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->find_value(key);
}
enum gkr_param_type gkr_params_get_type(struct gkr_params* params, const char* key)
{
    Check_Arg_NotNull(params, gkr_param_type_none);
    return gkr_param_type(reinterpret_cast<gkr::params*>(params)->get_type(key));
}
const char* gkr_params_get_string_value(struct gkr_params* params, size_t index, const char* def_val)
{
    Check_Arg_NotNull(params, def_val);
    return reinterpret_cast<gkr::params*>(params)->get_value(index, def_val);
}
double gkr_params_get_double_value(struct gkr_params* params, size_t index, double def_val)
{
    Check_Arg_NotNull(params, def_val);
    return reinterpret_cast<gkr::params*>(params)->get_value(index, def_val);
}
long long gkr_params_get_integer_value(struct gkr_params* params, size_t index, long long def_val)
{
    Check_Arg_NotNull(params, def_val);
    return reinterpret_cast<gkr::params*>(params)->get_value(index, def_val);
}
int gkr_params_get_boolean_value(struct gkr_params* params, size_t index, int def_val)
{
    Check_Arg_NotNull(params, def_val);
    return gkr_b2i(reinterpret_cast<gkr::params*>(params)->get_value(index, gkr_i2b(def_val)));
}
}

namespace gkr
{
inline std::size_t align(std::size_t size, std::size_t pitch) noexcept
{
    if(pitch <= 1) return size;

    if((size % pitch) != 0)
    {
        size /= pitch;
        size += 1;
        size *= pitch;
    }
    return size;
}

using type_t  = params::param_type_t;
using index_t = unsigned short;
using len_t   = unsigned short;
using key_t   = unsigned int;
using value_t = union
{
    unsigned
    long long   all;
    double      real;
    long long   integer;
    bool        boolean;
    index_t     child;
    struct      { key_t key; len_t len; } str;
};
struct node_t
{
    value_t     value;
    index_t     next;
    type_t      type;
    key_t       key;
};
static_assert( sizeof(node_t) == 16, "Keep it 16 bytes");
static_assert(alignof(node_t) ==  8, "Keep it  8 bytes");

struct head_t
{
    GKR_WARNING_DISABLE(4200, "GCC diagnostic ignored \"-Wpedantic\"", "GCC diagnostic ignored \"-Wzero-length-array\"")
    index_t     ref;
    len_t       len;
    char        str[0];
    GKR_WARNING_DEFAULT(4200)
};
static_assert( sizeof(head_t) ==  4, "Keep it  4 bytes");
static_assert(alignof(head_t) ==  2, "Keep it  2 bytes");

inline type_t determine_node_type(const char* sep)
{
    if((sep == nullptr) || (sep[1] == 0)) return type_t::param_type_none;

    return std::isdigit(sep[1])
        ? type_t::param_type_array
        : type_t::param_type_object;
}
inline bool is_correct_container_type(type_t type, int key_start_with_digit)
{
    if(type == type_t::param_type_array ) return (key_start_with_digit != 0);
    if(type == type_t::param_type_object) return (key_start_with_digit == 0);
    return false;
}
inline std::size_t calc_size_needed_for_text(std::size_t len)
{
    return align(sizeof(head_t) + len + 1, sizeof(head_t));
}

template<typename Functor>
bool params::traverse(bool skip_siblings, std::size_t index, Functor&& functor) const
{
    while(index != 0)
    {
        const node_t& node = get_node(index);

        if(functor(index, node)) return true;

        if((node.type == param_type_array) || (node.type == param_type_object))
        {
            if(traverse(false, node.value.child, functor)) return true;
        }

        if(skip_siblings) break;

        index = node.next;
    }
    return false;
}

params::params() noexcept(std::is_nothrow_default_constructible<buffer_t>::value)
{
    static_assert(alignof(node_t) <= alignof(buffer_t::allocator_traits::value_type), "Must be that way");
}

params::~params() noexcept(std::is_nothrow_destructible<buffer_t>::value)
{
    clear();
}

void params::copy_params(const params& other, std::size_t pitch)
{
    std::size_t count = 0;
    std::size_t size  = 0;
    std::size_t depth = 0;

    other.collect_info(false, m_root, 1, count, size, depth);

    if(count == 0) return;

    const std::size_t saved_pitch = m_pitch;
    clear();
    reserve(size, pitch);

    mirror_node(false, other, other.m_root, m_root);

    m_pitch = saved_pitch;
    m_root  = 1;
}

void params::clear() noexcept
{
    m_buff.clear();
    m_pitch  = DEFAULT_PITCH;
    m_offset = 0U;
    m_nodes  = 0U;
    m_root   = 0U;
}

void params::reserve(std::size_t size, std::size_t pitch)
{
    if(pitch != 0) m_pitch = pitch;

    size = align(size, m_pitch);

    const std::size_t old_capacity = m_buff.capacity();
    const std::size_t new_capacity = size;

    if(new_capacity <= old_capacity) return;

    const std::size_t size_to_move = m_nodes * sizeof(node_t);

    m_buff.reserve(new_capacity);
    m_buff.change_size(size);

    std::memmove(
        m_buff.data<char>() + new_capacity - size_to_move,
        m_buff.data<char>() + old_capacity - size_to_move,
        size_to_move);
}

params& params::reset_root(std::size_t index)
{
    Check_Arg_IsValid(index <= m_nodes, *this);

    if(index <= 1)
    {
        m_root = (m_nodes > 0) ? 1U : 0;
    }
    else
    {
        m_root = index;
    }
    return *this;
}

std::size_t params::insert_object(const char* key)
{
    const std::size_t index = append_node(key);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_object: break;
        case param_type_none  : node.type = param_type_object; break;
        default: return 0;
    }
    return index;
}

std::size_t params::insert_array(const char* key)
{
    const std::size_t index = append_node(key);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_array: break;
        case param_type_none : node.type = param_type_array; break;
        default: return 0;
    }
    return index;
}

std::size_t params::insert_null(const char* key)
{
    const std::size_t index = append_node(key);

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
    const std::size_t index = append_node(key);

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
    const std::size_t index = append_node(key);

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
    const std::size_t index = append_node(key);

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

    const std::size_t index = append_node(key);

    if(index == 0) return 0;

    switch(int(get_node(index).type))
    {
        case param_type_none  :
        case param_type_string: break;
        default: return 0;
    }
    const std::size_t len = std::strlen(value);
    const std::size_t ofs = insert_text(index, value, len);

    node_t& node = get_node(index);

    node.type          = param_type_string;
    node.value.str.key = key_t(ofs);
    node.value.str.len = len_t(len);
    return index;
}

std::size_t params::find_value(const char* key) const
{
    if(key == nullptr) return m_root;

    std::size_t root = m_root;
    
    if(*key == '/')
    {
        ++key;
        root = (m_nodes > 0) ? 1U : 0;
        if(*key == 0) return root;
    }
    const std::size_t index = search_node(key, root);

    return index;
}

type_t params::get_type(std::size_t index) const
{
    Check_Arg_IsValid(index <= m_nodes, param_type_none);

    if(index == 0) return param_type_none;

    type_t type = get_node(index).type;

    return type;
}

bool params::get_value(std::size_t index, bool def_val) const
{
    Check_Arg_IsValid(index <= m_nodes, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_boolean) return def_val;

    return node.value.boolean;
}

double params::get_value(std::size_t index, double def_val) const
{
    Check_Arg_IsValid(index <= m_nodes, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_double) return def_val;

    return node.value.real;
}

long long params::get_value(std::size_t index, long long def_val) const
{
    Check_Arg_IsValid(index <= m_nodes, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_integer) return def_val;

    return node.value.integer;
}

const char* params::get_value(std::size_t index, const char* def_val) const
{
    Check_Arg_IsValid(index <= m_nodes, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_string) return def_val;

    return get_text(node.value.str.key);
}

void params::get_info(std::size_t& count, std::size_t& size, std::size_t& depth) const
{
    count = 0;
    size  = 0;
    depth = 0;

    collect_info(false, m_root, 1, count, size, depth);
}

const node_t& params::get_node(std::size_t index) const
{
    return *(m_buff.data<node_t>(m_buff.capacity()) - index);
}

node_t& params::get_node(std::size_t index)
{
    return *(m_buff.data<node_t>(m_buff.capacity()) - index);
}

const char* params::get_text(std::size_t key) const
{
    return m_buff.as<head_t>(key).str;
}

bool params::keys_are_equal(const char* key, std::size_t len, std::size_t ofs) const
{
    const char* text = get_text(ofs);

    return !std::memcmp(text, key, len) && (text[len] == 0);
}

bool params::peek_array_pos(const char* key, std::size_t len, std::size_t& pos) const noexcept
{
    if(len > 9) return false;

    for(pos = 0; std::isdigit(*key); ++key, --len)
    {
        pos = (pos * 10) + std::size_t(*key - '0');
    }
    return (len == 0);
}

std::size_t params::insert_text(std::size_t index, const char* text, std::size_t len)
{
    const std::size_t need_size = calc_size_needed_for_text(len);

    const std::size_t busy_size = m_nodes * sizeof(node_t) + m_offset;
    const std::size_t free_size = m_buff.size() - busy_size;

    if(need_size > free_size)
    {
        reserve(need_size);
    }
    const std::size_t offset = m_offset;

    m_offset += need_size;

    head_t& head = m_buff.as<head_t>(offset);

    head.ref      = index_t(index);
    head.str[len] = 0;

    std::memcpy(head.str, text, len);

    return offset;
}

std::size_t params::insert_node()
{
    if(m_nodes >= 0x10000) return 0;

    const std::size_t need_size = sizeof(node_t);

    const std::size_t busy_size = m_nodes * sizeof(node_t) + m_offset;
    const std::size_t free_size = m_buff.size() - busy_size;

    if(need_size > free_size)
    {
        reserve(need_size);
    }
    return ++m_nodes;
}

std::size_t params::create_node(const char* key, std::size_t len, std::size_t prev, std::size_t parent, const char* sep)
{
    const std::size_t index = insert_node();
    if(index == 0) return 0;

    if(prev   != 0) get_node(prev  ).next        = index_t(index);
    if(parent != 0) get_node(parent).value.child = index_t(index);

    node_t& node = get_node(index);
    node.value.all = 0;
    node.next = 0;
    node.type = determine_node_type(sep);
    node.key  = (key == nullptr)
        ? key_t(len)
        : key_t(insert_text(index, key, len));
    return index;
}

std::size_t params::append_node(const char* key)
{
    if(key == nullptr)
    {
        if(m_root != 0) return m_root;

        return create_node(nullptr, 0, 0, 0, nullptr);
    }
    if(m_root == 0)
    {
        m_root = std::isdigit(*key)
            ? insert_array (nullptr)
            : insert_object(nullptr)
            ;
        Check_ValidState(m_root != 0, 0);
    }
    return lookup_node(key, m_root);
}

std::size_t params::lookup_node(const char* key, std::size_t parent)
{
    if(parent == 0) return 0;

    const int key_start_with_digit = std::isdigit(*key);

    std::size_t index;
    {
        const node_t& node = get_node(parent);

        if(!is_correct_container_type(node.type, key_start_with_digit)) return 0;

        index = node.value.child;
    }

    const char* sep = std::strchr(key, '/');
    const bool last = (sep == nullptr);

    const std::size_t len = !last
        ? std::size_t(sep - key)
        : std::strlen(key)
        ;
    if(len == 0) return 0;

    if(key_start_with_digit) // add value to array
    {
        std::size_t pos;
        if(!peek_array_pos(key, len, pos)) return 0;

        for(std::size_t prev = 0; ; parent = 0)
        {
            if(index == 0)
            {
                index = create_node(nullptr, pos, prev, parent, sep);
                break;
            }
            node_t& node = get_node(index);

            if(node.key == pos) break;

            prev  = index;
            index = node.next;
        }
    }
    else // add value to object
    {
        for(std::size_t prev = 0; ; parent = 0)
        {
            if(index == 0)
            {
                index = create_node(key, len, prev, parent, sep);
                break;
            }
            node_t& node = get_node(index);

            if(keys_are_equal(key, len, node.key)) break;

            prev  = index;
            index = get_node(index).next;
        }
    }
    if(last) return index;

    return lookup_node(sep + 1, index);
}

std::size_t params::mirror_node(bool has_name, const params& other, std::size_t other_parent, std::size_t self_parent)
{
    if(other_parent == 0) return 0;

    const char* key;
    std::size_t len;

    for(std::size_t self_prev = 0, other_index = other_parent; other_index != 0; self_parent = 0)
    {
        const node_t& other_node = other.get_node(other_index);

        if(!has_name)
        {
            key = nullptr;
            len = other_node.key;
        }
        else
        {
            const head_t& head = other.m_buff.as<head_t>(other_node.key);
            key = head.str;
            len = head.len;
        }
        const std::size_t self_index = create_node(key, len, self_prev, self_parent, nullptr);

        node_t& self_node = get_node(self_index);

        self_node.value.all = other_node.value.all;
        self_node.type      = other_node.type;

        switch(int(self_node.type))
        {
            case param_type_array : mirror_node(false, other, other_index, self_index); break;
            case param_type_object: mirror_node(false, other, other_index, self_index); break;
            case param_type_string:
                self_node.value.str.key = key_t(insert_text(self_index, other.get_text(other_node.value.str.key), other_node.value.str.len));
                break;
        }

        self_prev   = self_index;
        other_index = other_node.next;
    }
    return 0;
}

std::size_t params::search_node(const char* key, std::size_t parent) const
{
    if(parent == 0) return 0;

    const int key_start_with_digit = std::isdigit(*key);

    std::size_t index;
    {
        const node_t& node = get_node(parent);

        if(!is_correct_container_type(node.type, key_start_with_digit)) return 0;

        index = node.value.child;
    }

    const char* sep = std::strchr(key, '/');
    const bool last = (sep == nullptr);

    const std::size_t len = !last
        ? std::size_t(sep - key)
        : std::strlen(key)
        ;
    if(len == 0) return 0;

    if(key_start_with_digit) // search in array
    {
        std::size_t pos;
        if(!peek_array_pos(key, len, pos)) return 0;

        for( ; ; )
        {
            if(index == 0) return 0;

            const node_t& node = get_node(index);

            if(node.key == pos) break;

            index = node.next;
        }
    }
    else // search in object
    {
        for( ; ; )
        {
            if(index == 0) return 0;

            const node_t& node = get_node(index);

            if(keys_are_equal(key, len, node.key)) break;

            index = node.next;
        }
    }
    if(last) return index;

    return search_node(sep + 1, index);
}

void params::collect_info(bool has_name, std::size_t index, std::size_t level, std::size_t& count, std::size_t& size, std::size_t& depth) const
{
    while(index != 0)
    {
        ++count;

        if(depth < level) depth = level;

        const node_t& node = get_node(index);

        if(has_name)
        {
            size += calc_size_needed_for_text(m_buff.as<head_t>(node.key).len);
        }
        switch(int(node.type))
        {
            case param_type_array : collect_info(false, node.value.child, level + 1, count, size, depth); break;
            case param_type_object: collect_info(true , node.value.child, level + 1, count, size, depth); break;
            case param_type_string:
                size += calc_size_needed_for_text(node.value.str.len);
                break;
        }
        index = node.next;
    }
}

}
