#pragma once

#include "message.h"

#include <gkr/basic_thread_worker.h>
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

class logger final : public basic_thread_worker
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

    virtual size_t get_wait_objects_count() noexcept override;

    virtual waitable_object* get_wait_object(size_t index) override;

    virtual bool start() override;
    virtual void finish() override;

    virtual void on_action(action_id_t action, void* param, void* result) override;

    virtual void on_wait_timeout() override;
    virtual void on_wait_success(size_t index) override;

    virtual bool on_exception(bool can_continue, const std::exception* e) noexcept override;

public:
    bool change_log_queue(std::size_t max_queue_entries, std::size_t max_message_chars);

public:
    void set_severities(bool clear_existing, const name_id_pair* severities);
    void set_facilities(bool clear_existing, const name_id_pair* facilities);

    void set_severity(const name_id_pair& severity);
    void set_facility(const name_id_pair& facility);

public:
    bool add_consumer(std::shared_ptr<consumer> consumer);
    bool del_consumer(std::shared_ptr<consumer> consumer);

    void del_all_consumers();

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
    void register_thread(std::thread::id tid, const char* name);

    void sync_log_message(message_data& entry);

    void check_thread_registered();

    bool compose_message(message_data& msg, std::size_t cch, int severity, int facility, const char* format, std::va_list args);

    void process_message(message_data& msg);
    void prepare_message(message_data& msg);

    void consume_message(const message_data& msg);

    void process_pending_messages();

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
        ACTION_REGISTER_THREAD  ,
        ACTION_SYNC_LOG_MESSAGE , 
    };

    static constexpr unsigned max_name_cch = 16;
    struct thread_name_t { char name[max_name_cch] {0}; };

    using lockfree_queue_t = lockfree_queue<void, true, true, impl::queue_simple_synchronization<1,1>>;

private:
    objects_waiter m_producer_waiter;

    lockfree_queue_t m_log_queue;

    std::vector<std::shared_ptr<consumer>> m_consumers;

    std::unordered_map<unsigned short, const char*> m_severities;
    std::unordered_map<unsigned short, const char*> m_facilities;

    std::unordered_map<std::thread::id, thread_name_t> m_thread_names;
};

}
}
