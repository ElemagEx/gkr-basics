#pragma once

#include <gkr/log/log.hpp>
#include <gkr/log/logging.h>

#include <gkr/log/consumer.hpp>

#include <memory>
#include <utility>

GKR_LOG_API int gkr_log_add_consumer(void* instance, std::shared_ptr<gkr::log::consumer> consumer);
GKR_LOG_API int gkr_log_del_consumer(void* instance, std::shared_ptr<gkr::log::consumer> consumer);

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
        unsigned max_message_chars = 427, // = 427 [512 bytes - sizeof(gkr_log_message) - 17] //1 for NTS and 16 for msg id/instance (64bit)
        const name_id_pair* severities_infos = nullptr,
        const name_id_pair* facilities_infos = nullptr
        )
        : initialized(0 != gkr_log_init(name, max_queue_entries, max_message_chars, severities_infos, facilities_infos))
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
template<class Parent, typename Function, typename... Args>
class template_consumer : public Parent
{
    Function m_consume;

    virtual const char* compose_output(const message& msg, unsigned* len, bool colored) override
    {
        return m_consume(msg, len, colored);
    }
public:
    template_consumer(Function consume, Args&&... args) : Parent(std::forward<Args>(args)...), m_consume(consume)
    {
    }
};
}

template<class Parent, typename Function, typename... Args>
inline int add_consumer(void* instance, Function consume, Args&&... args)
{
    return gkr_log_add_consumer(
        instance,
        consumer_ptr_t(
            new impl::template_consumer<Parent, Function, Args...>(consume, std::forward<Args>(args)...))
        );
}

}
}
