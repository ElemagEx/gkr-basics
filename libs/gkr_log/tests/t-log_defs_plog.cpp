#include <gkr/log/logging.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <gkr/testing/log_defs.hpp>

#include <catch2/catch_test_macros.hpp>

#include <gkr/log/galery.hpp>
#include <gkr/log/defs/plog.h>

#define COMMON_SEVERITIES_INFOS PLOG_SEVERITIES_INFOS
constexpr gkr::log::name_id_pair g_severities[] = COMMON_SEVERITIES_INFOS;
constexpr gkr::log::name_id_pair g_facilities[] = COMMON_FACILITIES_INFOS;

static gkr::log::logging logging(nullptr, 0, 0, g_severities, g_facilities);

#include <thread>

TEST_CASE("logging.logger.defs.plog. main")
{
    gkr_log_del_all_consumers(nullptr);

//  plog_add_appender_color_console<plog_formatter_txt_utc>();

    plog_add_appender_rolling_file<plog_formatter_csv_utc>(nullptr, "text.log", 1024, 4);

    PLOGD << "Hello log!"; // short macro
    PLOG_DEBUG << "Hello log!"; // long macro
    PLOG(plog::debug) << "Hello log!"; // function-style macro

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
