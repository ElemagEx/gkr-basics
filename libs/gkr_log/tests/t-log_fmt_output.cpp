#include <gkr/log/galery.hpp>
#include <gkr/log/consumer.hpp>
#include <gkr/log/defs/plog.h>

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

//"$IS00$[Thread: $R>12$$TNAM$ ($R008$$T_ID$)][Severity: $P-10$$SNAM$][Facility: $P 04$$FNAM$][Source: $FILE$@$LINE$] $$ Text: $TEXT$$IS01$",

TEST_CASE("logging.logger.format.output. main")
{
    //gkr_log_del_all_consumers();
    //gkr_log_add_consumer(std::make_shared<gkr::log::app_console_consumer>());

    char buf1[256];
    char buf2[256];

    gkr_log_message msg { 12345LL, gkr::stamp_now(), plog::none, 5, "int main()", "main.cpp", 16U, 12U, "Test message", "Main", "Info", "Testing"};

    constexpr const char* args[] = {
        "", "",
        "$C097$$C101$", "$C000$",
        "$C091$"      , "$C000$",
        "$C093$"      , "$C000$",
        "", "",
        "$C096$"      , "$C000$",
        "$C096$"      , "$C000$",
    };

    int severities[] = { plog::fatal, plog::info, plog::verbose };

    for(int severity : severities)
    {
        msg.severity = severity;

        unsigned len1 = gkr_log_format_output_time(
            buf1,
            256,
            PLOG_CONSOLE_PATTERN,
            msg.stamp,
            0);
        CHECK(len1 > 0);
        REQUIRE(errno == 0);

        unsigned len2 = gkr_log_format_output_text(
            buf2,
            256,
            buf1,
            &msg,
            0,
            args,
            2,
            7
            );
        CHECK(len2 > 0);
        REQUIRE(errno == 0);

        std::cout << buf2 << std::endl;
    }
    std::cout << "END" << std::endl;
    //std::this_thread::yield();
}
