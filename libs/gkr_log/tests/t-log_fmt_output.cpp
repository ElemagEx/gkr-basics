#include <gkr/log/galery.hpp>
#include <gkr/log/consumer.hpp>

//#include <gkr/log/logging.hpp>
//#include <gkr/log/consumers/app_console_consumer.hpp>
//#include <gkr/testing/log_defs.hpp>
//#include <gkr/log/defs/generic.hpp>

#include <catch2/catch_test_macros.hpp>

//#define COMMON_SEVERITIES_INFOS LOG_SEVERITIES_INFOS
//constexpr gkr::log::name_id_pair g_severities[] = COMMON_SEVERITIES_INFOS;
//constexpr gkr::log::name_id_pair g_facilities[] = COMMON_FACILITIES_INFOS;
//static gkr::log::logging logging(g_severities, g_facilities);

//#include <thread>
#include <iostream>

#include <gkr/stamp.hpp>

TEST_CASE("logging.logger.format.output. main")
{
    //gkr_log_del_all_consumers();
    //gkr_log_add_consumer(std::make_shared<gkr::log::app_console_consumer>());

    char buf[256];

    gkr_log_message msg { 12345LL, gkr::stamp_now(), 3, 5, "int main()", "main.cpp", 16U, 12U, "Test message", "Main", "Info", "Testing"};

    unsigned len = gkr_log_format_output_text(buf, 256, "$C091$[Thread: $R>12$$TNAM$ ($R008$$T_ID$)][Severity: $P-10$$SNAM$][Facility: $P 04$$FNAM$][Source: $FILE$@$LINE$] $$ Text: $TEXT$$C000$", &msg, 0);

    std::cout << buf << std::endl << "END" << std::endl;

    CHECK(len > 0);
    CHECK(errno == 0);

    //std::this_thread::yield();
}
