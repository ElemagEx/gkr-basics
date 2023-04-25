#pragma once

#include <gkr/log/message.h>

#include <gkr/thread_worker.h>
#include <gkr/waitable_lockfree_queue.h>

#include <memory>
#include <vector>
#include <cstdarg>
#include <unordered_map>

namespace gkr
{
namespace log
{

struct name_id_pair;
class consumer;

class logger final : public thread_worker
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

    virtual std::chrono::nanoseconds get_wait_timeout() noexcept override;

    virtual std::size_t get_wait_objects_count() noexcept override;

    virtual waitable_object* get_wait_object(std::size_t index) noexcept override;

    virtual bool on_start() override;
    virtual void on_finish() override;

    virtual void on_action(action_id_t action, void* param, void* result) override;

    virtual void on_wait_timeout() override;
    virtual void on_wait_success(std::size_t index) override;

    virtual bool on_exception(except_method_t method, const std::exception* e) noexcept override;

public:
    bool change_log_queue(std::size_t max_queue_entries, std::size_t max_message_chars);

public:
    void set_severities(bool clear_existing, const name_id_pair* severities);
    void set_facilities(bool clear_existing, const name_id_pair* facilities);

    void set_severity(const name_id_pair& severity);
    void set_facility(const name_id_pair& facility);

public:
    using consumer_ptr_t = std::shared_ptr<consumer>;

    bool add_consumer(consumer_ptr_t consumer);
    bool del_consumer(consumer_ptr_t consumer);

    void del_all_consumers();

public:
    using tid_t = decltype(message::tid);

    void set_thread_name(const char* name, tid_t tid = 0);

public:
    bool log_message(bool wait, int severity, int facility, const char* format, std::va_list args);

private:
    struct message_data : public message
    {
        message_data() = delete;
       ~message_data() = delete;

        char buffer[1];
    };

private:
    void sync_log_message(message_data& entry);

    bool compose_message(message_data& msg, std::size_t cch, int severity, int facility, const char* format, std::va_list args);

    void process_message(message_data& msg);
    void prepare_message(message_data& msg);

    void consume_message(const message_data& msg);

    void process_pending_messages();

private:
    static bool init_consumer(consumer& consumer);
    static void done_consumer(consumer& consumer);

private:
    enum : action_id_t
    {
        ACTION_CHANGE_LOG_QUEUE ,
        ACTION_SET_SEVERITIES   ,
        ACTION_SET_FACILITIES   ,
        ACTION_SET_SEVERITY     ,
        ACTION_SET_FACILITY     ,
        ACTION_ADD_CONSUMER     ,
        ACTION_DEL_CONSUMER     ,
        ACTION_DEL_ALL_CONSUMERS,
        ACTION_SET_THREAD_NAME  ,
        ACTION_SYNC_LOG_MESSAGE ,
    };

    using lockfree_queue_t = lockfree_queue<void, true, impl::queue_wait_support<0,1>>;

private:
    lockfree_queue_t m_log_queue;

    std::vector<consumer_ptr_t> m_consumers;

    std::unordered_map<unsigned short, const char*> m_severities;
    std::unordered_map<unsigned short, const char*> m_facilities;

    std::unordered_map<tid_t, const char*> m_thread_ids;
};

}
}
