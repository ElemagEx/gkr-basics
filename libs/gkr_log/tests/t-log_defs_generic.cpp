#include <gkr/log/logging.hpp>
#include <gkr/log/consumers/dummy_consumer.hpp>
#include <gkr/log/consumers/app_console_consumer.hpp>

#include <gkr/testing/log_defs.hpp>

//#define GKR_LOG_USE_C_DEFS
#include <gkr/log/defs/generic.hpp>

#include <catch2/catch_test_macros.hpp>

#ifdef _WIN32
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
#include <winerror.h>
#endif
#include <gkr/log/formatters/win32_format_message.h>

#include <catch2/benchmark/catch_benchmark.hpp>

#define COMMON_SEVERITIES_INFOS LOG_SEVERITIES_INFOS
constexpr gkr::log::name_id_pair g_severities[] = COMMON_SEVERITIES_INFOS;
constexpr gkr::log::name_id_pair g_facilities[] = COMMON_FACILITIES_INFOS;

static gkr::log::logging logging(g_severities, g_facilities, 256);

#include <thread>

static int N = 0;

TEST_CASE("logging.logger.defs.generic. main")
{
    gkr_log_del_all_consumers();
    gkr_log_add_consumer(std::make_shared<gkr::log::dummy_consumer>());

#ifdef _WIN32
    gkr_log_win32_format_message(ERROR_INVALID_PASSWORD, LOG_SEVERITY_ERROR, FACILITY_NETWORK, "Windows Error: ");
#endif

    LOGV(FACILITY_NETWORK, "Hello There!");

    BENCHMARK("wait for msg to log") {
        //int last_msg_id =
        LOGV(FACILITY_NETWORK) << "Hello There test " << ++N << "!" << LOG_FINISH;

        //gkr_log_get_this_thread_llm_id(last_msg_id);
        return 0;
    };



#if 0
#ifndef GKR_LOG_USE_C_DEFS
    LOGV(FACILITY_NETWORK) << "Hello There!";
    LOGV(FACILITY_NETWORK, "Hello There!");
#ifdef __cpp_lib_format
    LOGV(FACILITY_NETWORK, "Hello {}!", "There");
#endif
#else
int flag = 1;
    LOGV_IF_(flag < 8, FACILITY_NETWORK, "Hello %s!", "There");
#endif
#endif

    INFO("Number of iterations = " << N);

    std::this_thread::yield();
}
