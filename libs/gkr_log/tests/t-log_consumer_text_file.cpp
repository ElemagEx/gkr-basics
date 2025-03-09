#include <gkr/log/logging.hpp>
#include <gkr/log/consumers/text_file_consumer.hpp>
#include <gkr/log/defs/generic.hpp>

#include <gkr/testing/log_defs.hpp>

#include <catch2/catch_test_macros.hpp>

#define COMMON_SEVERITIES_INFOS LOG_SEVERITIES_INFOS
constexpr gkr::log::name_id_pair g_severities[] = COMMON_SEVERITIES_INFOS;
constexpr gkr::log::name_id_pair g_facilities[] = COMMON_FACILITIES_INFOS;

static gkr::log::logging logging(nullptr, 0, 0, g_severities, g_facilities);

#include <thread>

TEST_CASE("logging.logger. main")
{
    gkr_log_del_all_consumers(nullptr);
    gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::text_file_consumer>());

    gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First log message");

    std::thread t([] () {
        gkr_log_set_current_thread_name("gkr-bar");
        gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread message");
    });
    t.join();

    gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second log message");
}
