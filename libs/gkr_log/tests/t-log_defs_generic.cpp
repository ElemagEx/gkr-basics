#include <gkr/defs.hpp>

#include <gkr/log/logging.hpp>
#include <gkr/log/consumers/dummy_consumer.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <gkr/testing/log_defs.hpp>

#define LOG_THRESHOLD_LEVEL LOG_SEVERITY_VERBOSE
//#define GENERIC_LOG_USE_C_DEFS
#define GENERIC_LOG_FACILITY_KEEP
#include <gkr/log/galery.hpp>
#include <gkr/log/defs/generic.hpp>

#include <catch2/catch_test_macros.hpp>

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
#include <winerror.h>
#endif
#include <gkr/log/formatters/win32_format_message.hpp>

#include <catch2/benchmark/catch_benchmark.hpp>

#define COMMON_SEVERITIES_INFOS LOG_SEVERITIES_INFOS
constexpr gkr::log::name_id_pair g_severities[] = COMMON_SEVERITIES_INFOS;
constexpr gkr::log::name_id_pair g_facilities[] = COMMON_FACILITIES_INFOS;

static gkr::log::logging logging(nullptr, 0, 256, g_severities, g_facilities);

#include <thread>

static int N = 0;

TEST_CASE("logging.logger.defs.generic. main")
{
    using namespace gkr::log;

    gkr_log_del_all_consumers(nullptr);
    gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::app_console_consumer>());

#ifdef _WIN32
    gkr_log_win32_format_message(nullptr, ERROR_INVALID_PASSWORD, LOG_SEVERITY_ERROR, FACILITY_NETWORK, "Windows Error (%1!u!): ");
#endif

    LOGV(FACILITY_NETWORK, "Hello There!");
#if 0
    BENCHMARK("wait for msg to log") {
        //int last_msg_id =
        LOGV(FACILITY_NETWORK) << "Hello There test " << ++N << "!" << LOG_FINISH;

        //gkr_log_get_current_thread_llm_id(last_msg_id);
        return 0;
    };
#endif

#ifndef GENERIC_LOG_USE_C_DEFS
    LOGV(FACILITY_NETWORK) << "Hello There from stream!";
    LOGI(FACILITY_NETWORK) << "Hello There from stream!";
    LOGV(FACILITY_NETWORK, "Hello There from simple!");
    LOGI(FACILITY_NETWORK, "Hello There from simple!");
#ifdef __cpp_lib_format
    LOGV(FACILITY_NETWORK, "Hello {}!", "There");
#endif
#else
int flag = 1;
    LOGV_IF_(flag < 8, FACILITY_NETWORK, "Hello %s!", "There");

    LOGI_IF_(flag < 8, FACILITY_NETWORK, "Hello %s!", "There");
    LOGW_IF_(flag < 8, FACILITY_NETWORK, "Hello %s!", "There");
#endif

    INFO("Number of iterations = " << N);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
