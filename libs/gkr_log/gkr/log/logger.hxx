#pragma once

#include <gkr/log/consumer.hpp>
#include <gkr/log/logging.hpp>

#include <gkr/container/raw_buffer.hpp>

#include <gkr/concurency/worker_thread.hpp>
#include <gkr/concurency/waitable_event.hpp>
#include <gkr/concurency/waitable_lockfree_queue.hpp>

#include <list>
#include <vector>
#include <memory>
#include <unordered_map>

struct gkr_log_name_id_pair;

namespace gkr
{
namespace log
{

struct channel_info
{
    const char* name {""};
    int         threshold {100};
};
struct source_location
{
    const char* func;
    const char* file;
    unsigned    line;
};

using name_id_pair = gkr_log_name_id_pair;

class consumer;

class logger final : public worker_thread
{
    logger           (const logger&) noexcept = delete;
    logger& operator=(const logger&) noexcept = delete;

    logger           (logger&&) noexcept = delete;
    logger& operator=(logger&&) noexcept = delete;

public:
    logger();
    virtual ~logger() noexcept(DIAG_NOEXCEPT) override;

private:
    virtual const char* get_name() noexcept override;

    virtual long long get_wait_timeout_ns() noexcept override;

    virtual std::size_t get_waitable_objects_count() noexcept override;

    virtual waitable_object& get_waitable_object(std::size_t index) noexcept override;

    virtual bool on_start() override;
    virtual void on_finish() override;

    virtual void on_cross_action(action_id_t action, void* param, void* result) override;
    virtual void on_queue_action(action_id_t action, void* data) override;

    virtual void on_wait_timeout() override;
    virtual void on_wait_success(std::size_t index) override;

    virtual bool on_exception(except_method_t method, const std::exception* e) noexcept override;

public:
    void* add_channel(
        bool primary,
        const char* name,
        std::size_t max_queue_entries,
        std::size_t max_message_chars,
        const struct gkr_log_name_id_pair* severities_infos,
        const struct gkr_log_name_id_pair* facilities_infos
        );
    bool del_channel(void* channel);

    void* get_channel(const char* name);

public:
    unsigned get_max_queue_entries() const
    {
        return unsigned(m_log_queue.capacity());
    }
    unsigned get_max_message_chars() const
    {
        return (m_log_queue.element_size() <= (sizeof(message_head)+1))
            ? 0U
            : unsigned(m_log_queue.element_size() - (sizeof(message_head)+1));
    }

public:
    bool change_log_queue(std::size_t max_queue_entries, std::size_t max_message_chars);

    bool change_name(void* channel, const char* name);

public:
    bool set_severities(void* channel, bool clear_existing, const name_id_pair* severities_infos);
    bool set_facilities(void* channel, bool clear_existing, const name_id_pair* facilities_infos);

    bool set_severity(void* channel, const name_id_pair& severity_info);
    bool set_facility(void* channel, const name_id_pair& facility_info);

public:
    void set_severity_threshold(void* channel, int threshold)
    {
        get_data(channel).threshold = threshold;
    }
    int get_severity_threshold(void* channel)
    {
        return get_data(channel).threshold;
    }

public:
    int  add_consumer(void* channel, consumer_ptr_t consumer, int id);
    bool del_consumer(void* channel, consumer_ptr_t consumer, int id);

    bool del_all_consumers(void* channel);

public:
    using tid_t = long long;

    bool set_thread_name(int* ptr, const char* name, tid_t tid = 0);

public:
    void* start_log_message(void* channel, int severity, char*& buf, unsigned& cch);
    int  cancel_log_message(void* context, void* channel);
    int  finish_log_message(void* context, void* channel, const source_location* location, int severity, int facility);

    int log_message(void* channel, const source_location* location, int severity, int facility, const char* format, va_list args);

public:
    const char* format_output(
        const char* fmt,
        const struct gkr_log_message& msg,
        int flags,
        const char* const* args,
        unsigned rows,
        unsigned cols,
        unsigned* len
        );

private:
    struct message_head : message
    {
        int   id;
    //  int   free-4-bytes (in 64bit build)
        void* channel;
    };
    struct message_data : message_head
    {
        char buf[1];
    };

private:
    void compose_message(message_data& msg, std::size_t cch, void* channel, const source_location* location, int severity, int facility, const char* format, va_list args);

    void process_message(message_data& msg);

    void prepare_message(message_data& msg);
    void consume_message(const message_data& msg);

    void process_all_messages();
    bool process_next_message();

private:
    consumer_ptr_t find_consumer(int id);

    static bool init_consumer(consumer& consumer);
    static void done_consumer(consumer& consumer);

private:
    enum : action_id_t
    {
        ACTION_ADD_CHANNEL      ,
        ACTION_DEL_CHANNEL      ,
        ACTION_GET_CHANNEL      ,
        ACTION_CHANGE_LOG_QUEUE ,
        ACTION_SET_SEVERITIES   ,
        ACTION_SET_FACILITIES   ,
        ACTION_SET_SEVERITY     ,
        ACTION_SET_FACILITY     ,
        ACTION_ADD_CONSUMER     ,
        ACTION_DEL_CONSUMER     ,
        ACTION_DEL_ALL_CONSUMERS,
        ACTION_SET_THREAD_NAME  ,
    };

    using log_queue_t = waitable_lockfree_queue<void, true, true>;

    struct consumer_info
    {
        consumer_ptr_t consumer;
        bool           reentry_guard {false};
        int            id {0};
    };
    struct channel_data : channel_info
    {
        std::vector<consumer_info> consumers;

        std::unordered_map<int, const char*> severities;
        std::unordered_map<int, const char*> facilities;
    };
    channel_data& get_data(void* channel)
    {
        return (channel == nullptr)
            ? m_primary
            : *static_cast<channel_data*>(channel);
    }

private:
    log_queue_t m_log_queue;

    channel_data m_primary;

    std::list  <channel_data>  m_channels;
    std::vector<consumer_info> m_consumers;

    std::unordered_map<tid_t, const char*> m_thread_ids;
    std::unordered_map<tid_t, int*>        m_msg_id_ptr;

    std::atomic<int> m_msg_id {0};

    int m_consumer_id {0};
    int m_ref_count   {0};

    raw_buffer m_fmt;
    raw_buffer m_txt;
};

}
}
