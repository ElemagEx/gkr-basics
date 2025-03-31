#include <gkr/defs.hpp>

#include <gkr/params.hpp>

#include <gkr/diagnostics.hpp>
#include <gkr/misc/c_allocator.hpp>

#include <cstring>
#include <cctype>

namespace gkr
{
static void lifecycle(gkr::params* params, void* param)
{
    if(param != nullptr)
    {
        params->m_param = param;
    }
    else if(params->m_param == nullptr)
    {
        delete params;
    }
    else
    {
        using free_proc_t = void (*)(void*);
        free_proc_t free_proc = static_cast<free_proc_t>(params->m_param);
        params->~params();
        free_proc(params);
    }
}
}

extern "C"
{

struct gkr_params* gkr_params_create(int multithreaded, void* (*alloc_proc)(size_t), void (*free_proc)(void*))
{
    using Allocator = gkr::misc::c_allocator<std::max_align_t>;

    Allocator allocator(alloc_proc, free_proc);

    gkr::params* params;

    if(allocator.invalid())
    {
        if(multithreaded)
        {
            params = new gkr::multithreaded_params();
        }
        else
        {
            params = new gkr::singlethreaded_params();
        }
    }
    else
    {
        if(multithreaded)
        {
            using params_t = gkr::basic_multithreaded_params<Allocator>;
            params = static_cast<params_t*>((*alloc_proc)(sizeof(params_t)));
            new (params) params_t(allocator);
        }
        else
        {
            using params_t = gkr::basic_singlethreaded_params<Allocator>;
            params = static_cast<params_t*>((*alloc_proc)(sizeof(params_t)));
            new (params) params_t(allocator);
        }
        gkr::lifecycle(params, free_proc);
    }
    return reinterpret_cast<struct gkr_params*>(params);
}
int gkr_params_destroy(struct gkr_params* params)
{
    Check_Arg_NotNull(params, false);
    lifecycle(reinterpret_cast<gkr::params*>(params), nullptr);
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
size_t gkr_params_add_object(struct gkr_params* params, const char* key, size_t root)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->add_object(key, root);
}
size_t gkr_params_add_array(struct gkr_params* params, const char* key, size_t root)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->add_array(key, root);
}
size_t gkr_params_add_null(struct gkr_params* params, const char* key, size_t root)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->add_null(key, root);
}
size_t gkr_params_set_string_value(struct gkr_params* params, const char* key, const char* value, size_t root, int overwrite)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->set_value(key, value, root, gkr_i2b(overwrite));
}
size_t gkr_params_set_double_value(struct gkr_params* params, const char* key, double value, size_t root, int overwrite)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->set_value(key, value, root, gkr_i2b(overwrite));
}
size_t gkr_params_set_integer_value(struct gkr_params* params, const char* key, long long value, size_t root, int overwrite)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->set_value(key, value, root, gkr_i2b(overwrite));
}
size_t gkr_params_set_boolean_value(struct gkr_params* params, const char* key, int value, size_t root, int overwrite)
{
    Check_Arg_NotNull(params, 0);
    return reinterpret_cast<gkr::params*>(params)->set_value(key, gkr_i2b(value), root, gkr_i2b(overwrite));
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
    key_t       string;
    index_t     child;
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

struct text_t
{
    GKR_WARNING_DISABLE(4200, "GCC diagnostic ignored \"-Wpedantic\"", "GCC diagnostic ignored \"-Wzero-length-array\"")
    index_t     ref;
    len_t       len;
    char        ptr[0];
    GKR_WARNING_DEFAULT(4200)
};
static_assert( sizeof(text_t) ==  4, "Keep it  4 bytes");
static_assert(alignof(text_t) ==  2, "Keep it  2 bytes");

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
inline std::size_t calc_size_need_for_text(std::size_t len)
{
    return align(sizeof(text_t) + len + 1, sizeof(text_t));
}
inline void check_root(std::size_t& root, std::size_t nodes)
{
    Assert_Check(root <= nodes);
    if(root == 0) root = (nodes > 0) ? 1U : 0;
}

params::params(std::size_t pitch) noexcept : m_pitch(pitch)
{
}

params::~params()
{
}

bool params::get_info(std::size_t root, std::size_t& count, std::size_t& size, std::size_t& depth) const
{
    std::shared_lock<const params> lock(*this);

    count = 0;
    size  = 0;
    depth = 0;

    Check_Arg_IsValid(root <= m_count, false);

    check_root(root, m_count);

    collect_info(false, root, 1, count, size, depth);

    size += count * sizeof(node_t);
    return true;
}

bool params::copy_params(const params& other, std::size_t root, std::size_t pitch)
{
    std::unique_lock<      params> lock1(*this);//???
    std::shared_lock<const params> lock2(other);//???

    Check_Arg_IsValid(root <= other.m_count, false);

    check_root(root, other.m_count);

    std::size_t count = 0;
    std::size_t size  = 0;
    std::size_t depth = 0;

    other.collect_info(false, root, 1, count, size, depth);

    if(count == 0) return true;

    size += count * sizeof(node_t);

    m_offset = 0U;
    m_count  = 0U;
    reserve(size, pitch);

    mirror_node(false, other, root, 0);

    return true;
}

void params::reserve(std::size_t size, std::size_t pitch)
{
    std::unique_lock<params> lock(*this);

    if(pitch != 0) pitch = m_pitch;

    const std::size_t old_size = get_memory_footprint();
    const std::size_t new_size = align(size, pitch);

    if(new_size <= old_size) return;

    m_texts = realloc(new_size);
    m_nodes = reinterpret_cast<node_t*>(m_texts + new_size);

    const std::size_t size_to_move = m_count * sizeof(node_t);

    std::memmove(
        m_texts + new_size - size_to_move,
        m_texts + old_size - size_to_move,
        size_to_move);
}

void params::compact()
{
    std::unique_lock<params> lock(*this);
}

void params::clear()
{
    std::unique_lock<params> lock(*this);

    m_count  = 0U;
    m_offset = 0U;

    m_nodes  = nullptr;
    m_texts  = nullptr;
}

std::size_t params::add_object(const char* key, std::size_t root)
{
    std::unique_lock<params> lock(*this);

    Check_Arg_IsValid(root <= m_count, 0);

    check_root(root, m_count);

    const std::size_t index = append_node(key, root);

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

std::size_t params::add_array(const char* key, std::size_t root)
{
    std::unique_lock<params> lock(*this);

    Check_Arg_IsValid(root <= m_count, 0);

    const std::size_t index = append_node(key, root);

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

std::size_t params::add_null(const char* key, std::size_t root)
{
    std::unique_lock<params> lock(*this);

    Check_Arg_IsValid(root <= m_count, 0);

    const std::size_t index = append_node(key, root);

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

std::size_t params::set_value(const char* key, bool value, std::size_t root, bool overwrite)
{
    std::unique_lock<params> lock(*this);

    const std::size_t index = append_node(key, root);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_boolean: break;
        case param_type_none   : node.type = param_type_boolean; overwrite = true; break;
        default: return 0;
    }
    if(overwrite)
    {
        node.value.boolean = value;
    }
    return index;
}

std::size_t params::set_value(const char* key, double value, std::size_t root, bool overwrite)
{
    std::unique_lock<params> lock(*this);

    const std::size_t index = append_node(key, root);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_double: break;
        case param_type_none  : node.type = param_type_double; overwrite = true; break;
        default: return 0;
    }
    if(overwrite)
    {
        node.value.real = value;
    }
    return index;
}

std::size_t params::set_value(const char* key, long long value, std::size_t root, bool overwrite)
{
    std::unique_lock<params> lock(*this);

    const std::size_t index = append_node(key, root);

    if(index == 0) return 0;

    node_t& node = get_node(index);

    switch(int(node.type))
    {
        case param_type_integer: break;
        case param_type_none   : node.type = param_type_integer; overwrite = true; break;
        default: return 0;
    }
    if(overwrite)
    {
        node.value.integer = value;
    }
    return index;
}

std::size_t params::set_value(const char* key, const char* value, std::size_t root, bool overwrite)
{
    std::unique_lock<params> lock(*this);

    Check_Arg_NotNull(value, 0);

    const std::size_t index = append_node(key, root);

    if(index == 0) return 0;

    switch(int(get_node(index).type))
    {
        case param_type_string: break;
        case param_type_none  : overwrite = true; break;
        default: return 0;
    }
    if(overwrite)
    {
        const std::size_t len = std::strlen(value);
        const std::size_t ofs = insert_text(index, value, len);

        node_t& node = get_node(index);

        node.type         = param_type_string;
        node.value.string = key_t(ofs);
    }
    return index;
}

std::size_t params::find_value(const char* key, std::size_t root) const
{
    std::shared_lock<const params> lock(*this);

    Check_Arg_IsValid(root <= m_count);

    check_root(root, m_count);

    if(key == nullptr) return root;

    const std::size_t index = search_node(key, root);

    return index;
}

type_t params::get_type(std::size_t index) const
{
    std::shared_lock<const params> lock(*this);

    Check_Arg_IsValid(index <= m_count, param_type_none);

    if(index == 0) return param_type_none;

    type_t type = get_node(index).type;

    return type;
}

bool params::get_value(std::size_t index, bool def_val) const
{
    std::shared_lock<const params> lock(*this);

    Check_Arg_IsValid(index <= m_count, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_boolean) return def_val;

    return node.value.boolean;
}

double params::get_value(std::size_t index, double def_val) const
{
    std::shared_lock<const params> lock(*this);

    Check_Arg_IsValid(index <= m_count, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_double) return def_val;

    return node.value.real;
}

long long params::get_value(std::size_t index, long long def_val) const
{
    std::shared_lock<const params> lock(*this);

    Check_Arg_IsValid(index <= m_count, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_integer) return def_val;

    return node.value.integer;
}

const char* params::get_value(std::size_t index, const char* def_val) const
{
    std::shared_lock<const params> lock(*this);

    Check_Arg_IsValid(index <= m_count, def_val);

    if(index == 0) return def_val;

    const node_t& node = get_node(index);

    if(node.type != param_type_string) return def_val;

    return get_text(node.value.string).ptr;
}

const node_t& params::get_node(std::size_t index) const
{
    return *(m_nodes - index);
}

node_t& params::get_node(std::size_t index)
{
    return *(m_nodes - index);
}

const text_t& params::get_text(std::size_t offset) const
{
    Assert_Check((offset % alignof(text_t)) == 0);

    return *reinterpret_cast<const text_t*>(m_texts + offset);
}

text_t& params::get_text(std::size_t offset)
{
    Assert_Check((offset % alignof(text_t)) == 0);

    return *reinterpret_cast<text_t*>(m_texts + offset);
}

bool params::keys_are_equal(const char* key, std::size_t len, std::size_t ofs) const
{
    const text_t& text = get_text(ofs);

    return !std::memcmp(text.ptr, key, len) && (text.ptr[len] == 0);
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

std::size_t params::insert_text(std::size_t index, const char* ptr, std::size_t len)
{
    const std::size_t need_size = calc_size_need_for_text(len);

    const std::size_t buff_size = get_memory_footprint();
    const std::size_t busy_size = m_count * sizeof(node_t) + m_offset;
    const std::size_t free_size = buff_size - busy_size;

    if(need_size > free_size)
    {
        reserve(buff_size + need_size - free_size);
    }
    const std::size_t offset = m_offset;

    m_offset += need_size;

    text_t& text = get_text(offset);

    text.ref      = index_t(index);
    text.len      = len_t(len);
    text.ptr[len] = 0;

    std::memcpy(text.ptr, ptr, len);

    return offset;
}

std::size_t params::insert_node()
{
    if(m_count >= 0x10000) return 0;

    const std::size_t need_size = sizeof(node_t);

    const std::size_t buff_size = get_memory_footprint();
    const std::size_t busy_size = m_count * sizeof(node_t) + m_offset;
    const std::size_t free_size = buff_size - busy_size;

    if(need_size > free_size)
    {
        reserve(buff_size + need_size - free_size);
    }
    return ++m_count;
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

std::size_t params::append_node(const char* key, std::size_t root)
{
    if(key == nullptr)
    {
        if(root != 0) return root;

        return create_node(nullptr, 0, 0, 0, nullptr);
    }
    if(root == 0)
    {
        root = create_node(nullptr, 0, 0, 0, nullptr);
        if(root == 0) return 0;
        get_node(root).type = std::isdigit(*key)
            ? param_type_array
            : param_type_object;
    }
    return lookup_node(key, root);
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

        if(has_name)
        {
            const text_t& other_text = other.get_text(other_node.key);
            key = other_text.ptr;
            len = other_text.len;
        }
        else
        {
            key = nullptr;
            len = other_node.key;
        }
        const std::size_t self_index = create_node(key, len, self_prev, self_parent, nullptr);

        node_t& self_node = get_node(self_index);

        self_node.value.all = other_node.value.all;
        self_node.type      = other_node.type;

        switch(int(self_node.type))
        {
            case param_type_array : mirror_node(false, other, other_index, self_index); break;
            case param_type_object: mirror_node(true , other, other_index, self_index); break;
            case param_type_string:
                {
                    const text_t& other_text = other.get_text(other_node.value.string);

                    self_node.value.string = key_t(insert_text(self_index, other_text.ptr, other_text.len));
                }
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
            const text_t& head = *reinterpret_cast<text_t*>(m_texts + node.key);
            size += calc_size_need_for_text(head.len);
        }
        switch(int(node.type))
        {
            case param_type_array : collect_info(false, node.value.child, level + 1, count, size, depth); break;
            case param_type_object: collect_info(true , node.value.child, level + 1, count, size, depth); break;
            case param_type_string:
                {
                    const text_t& text = get_text(node.value.string);

                    size += calc_size_need_for_text(text.len);
                }
                break;
        }
        index = node.next;
    }
}

}
