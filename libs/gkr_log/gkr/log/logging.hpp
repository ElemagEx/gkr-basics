#pragma once

#include <gkr/capi/log/logging.h>

#include <gkr/log/log.hpp>
#include <gkr/log/consumer.hpp>

#include <memory>
#include <utility>

GKR_LOG_API int gkr_log_add_consumer(void* channel, std::shared_ptr<gkr::log::consumer> consumer);
GKR_LOG_API int gkr_log_del_consumer(void* channel, std::shared_ptr<gkr::log::consumer> consumer);

namespace gkr
{
namespace log
{
using name_id_pair = gkr_log_name_id_pair;

using consumer_ptr_t = std::shared_ptr<consumer>;

struct logging final
{
    const bool initialized;

    logging(
        const char* name = nullptr,
        unsigned max_queue_entries = 32,
        unsigned max_message_chars = 427, // = 427 [512 bytes - sizeof(gkr_log_message) - 17] //1 for NTS and 16 for msg id/channel (64bit)
        const name_id_pair* severities_infos = nullptr,
        const name_id_pair* facilities_infos = nullptr
        )
        : initialized(0 != gkr_log_init(name, max_queue_entries, max_message_chars, severities_infos, facilities_infos, 0))
    {
    }
    ~logging()
    {
        gkr_log_done();
    }

private:
    logging           (const logging& other) noexcept = delete;
    logging& operator=(const logging&      ) noexcept = delete;

    logging           (logging&& other) noexcept = delete;
    logging& operator=(logging&&      ) noexcept = delete;
};

namespace impl
{
template<class Consumer, typename FuncFilter, typename... Args>
class template_filter_consumer : public Consumer
{
    template_filter_consumer           (const template_filter_consumer&) noexcept = delete;
    template_filter_consumer& operator=(const template_filter_consumer&) noexcept = delete;

    template_filter_consumer           (template_filter_consumer&&) noexcept = delete;
    template_filter_consumer& operator=(template_filter_consumer&&) noexcept = delete;

    FuncFilter m_filter;

    virtual bool filter_log_message(const message& msg) override
    {
        return m_filter(msg);
    }
public:
    template_filter_consumer(FuncFilter filter, Args&&... args) : Consumer(std::forward<Args>(args)...), m_filter(filter)
    {
    }
};
template<class Consumer, typename FuncCompose, typename... Args>
class template_composer_consumer : public Consumer
{
    template_composer_consumer           (const template_composer_consumer&) noexcept = delete;
    template_composer_consumer& operator=(const template_composer_consumer&) noexcept = delete;

    template_composer_consumer           (template_composer_consumer&&) noexcept = delete;
    template_composer_consumer& operator=(template_composer_consumer&&) noexcept = delete;

    FuncCompose m_compose;

    virtual const char* compose_output(const message& msg, unsigned* len, bool colored) override
    {
        return m_compose(msg, len, colored);
    }
public:
    template_composer_consumer(FuncCompose compose, Args&&... args) : Consumer(std::forward<Args>(args)...), m_compose(compose)
    {
    }
};
template<class TextFileConsumer, typename FuncCompose, typename FuncEvent1, typename FuncEvent2, typename FuncEvent3, typename FuncEvent4, typename... Args>
class template_text_file_consumer : public TextFileConsumer
{
    template_text_file_consumer           (const template_text_file_consumer&) noexcept = delete;
    template_text_file_consumer& operator=(const template_text_file_consumer&) noexcept = delete;

    template_text_file_consumer           (template_text_file_consumer&&) noexcept = delete;
    template_text_file_consumer& operator=(template_text_file_consumer&&) noexcept = delete;

    FuncCompose m_compose;
    FuncEvent1  m_on_opened;
    FuncEvent2  m_on_closing;
    FuncEvent3  m_on_enter;
    FuncEvent4  m_on_leave;

    virtual const char* compose_output(const message& msg, unsigned* len, bool colored) override
    {
        return m_compose(msg, len, colored);
    }
    virtual void on_file_opened     () override { m_on_opened (*this); }
    virtual void on_file_closing    () override { m_on_closing(*this); }
    virtual void on_enter_file_write() override { m_on_enter  (*this); }
    virtual void on_leave_file_write() override { m_on_leave  (*this); }
public:
    template_text_file_consumer(
        FuncCompose compose,
        FuncEvent1 on_opened,
        FuncEvent2 on_closing,
        FuncEvent3 on_enter,
        FuncEvent4 on_leave,
        Args&&... args
        )
        : TextFileConsumer(std::forward<Args>(args)...)
        , m_compose   (compose   )
        , m_on_opened (on_opened )
        , m_on_closing(on_closing)
        , m_on_enter  (on_enter  )
        , m_on_leave  (on_leave  )
    {
    }
};
}

template<class Consumer, typename FuncFilter, typename... Args>
inline int add_filter_consumer(void* channel, FuncFilter filter, Args&&... args)
{
    return gkr_log_add_consumer(
        channel,
        consumer_ptr_t(
            new impl::template_filter_consumer<Consumer, FuncFilter, Args...>(
                filter,
                std::forward<Args>(args)...)
                )
        );
}

template<class Consumer, typename FuncCompose, typename... Args>
inline int add_composer_consumer(void* channel, FuncCompose compose, Args&&... args)
{
    return gkr_log_add_consumer(
        channel,
        consumer_ptr_t(
            new impl::template_composer_consumer<Consumer, FuncCompose, Args...>(
                compose,
                std::forward<Args>(args)...)
                )
        );
}

template<class TextFileConsumer, typename FuncCompose, typename FuncEvent1, typename FuncEvent2, typename FuncEvent3, typename FuncEvent4, typename... Args>
inline int add_text_file_consumer(
    void* channel,
    FuncCompose compose,
    FuncEvent1 on_opened,
    FuncEvent2 on_closing,
    FuncEvent3 on_enter,
    FuncEvent4 on_leave,
    Args&&... args
    )
{
    return gkr_log_add_consumer(
        channel,
        consumer_ptr_t(
            new impl::template_text_file_consumer<TextFileConsumer, FuncCompose, FuncEvent1, FuncEvent2, FuncEvent3, FuncEvent4, Args...>(
                compose,
                on_opened,
                on_closing,
                on_enter,
                on_leave,
                std::forward<Args>(args)...)
                )
        );
}

}
}
