#include <gkr/log/logging.h>
#include <gkr/log/consumers/text_file_consumer.h>

#include <gkr/testing/log_defs.h>

#include <catch2/catch_test_macros.hpp>

constexpr gkr::log::name_id_pair g_severities[] = COMMON_SEVERITIES;
constexpr gkr::log::name_id_pair g_facilities[] = COMMON_FACILITIES;

static gkr::log::logging logging(g_severities, g_facilities);

#include <thread>

TEST_CASE("logging.logger. main")
{
    gkr_log_del_all_consumers();
    gkr_log_add_consumer(std::make_shared<gkr::log::text_file_consumer>());

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First log message");

    std::thread t([] () {
        gkr_log_set_this_thread_name("gkr-bar");
        gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread message");
    });
    t.join();

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second log message");
}
