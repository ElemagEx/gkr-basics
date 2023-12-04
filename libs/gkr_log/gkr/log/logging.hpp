#pragma once

#include <gkr/log/log.hpp>
#include <gkr/log/logging.h>

#include <memory>

namespace gkr
{
namespace log
{

using name_id_pair = gkr_log_name_id_pair;
class consumer;

struct logging final
{
    const bool initialized;

    logging(
        const name_id_pair* severities_infos = nullptr,
        const name_id_pair* facilities_infos = nullptr,
        unsigned max_queue_entries = 16,
        unsigned max_message_chars = 967 // = 967 [1024 - sizeof(gkr_log_message) - 1] //1 for null terminated character
        )
        : initialized(0 != gkr_log_init(severities_infos, facilities_infos, max_queue_entries, max_message_chars))
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

}
}

GKR_LOG_API int gkr_log_add_consumer(std::shared_ptr<gkr::log::consumer> consumer);
GKR_LOG_API int gkr_log_del_consumer(std::shared_ptr<gkr::log::consumer> consumer);
