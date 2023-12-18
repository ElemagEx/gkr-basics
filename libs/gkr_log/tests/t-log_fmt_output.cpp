#include <gkr/log/galery.hpp>

#include <gkr/log/defs/plog.h>
#include <gkr/stamp.hpp>

#include <catch2/catch_test_macros.hpp>

#include <iostream>
#include <cstring>

gkr_log_message msg { 12345LL, gkr::stamp_now(), plog::none, 5, "int main()", "main.cpp", 16U, 12U, "Test message", "Default", "Main", "Info", "Testing"};

TEST_CASE("logging.logger.format.output. sample")
{
    const char* fmt = "<IS00>[Thread: <R~12><TNAM> (<R008><T_ID>)][Severity: <P-10><SNAM>][Facility: <P 04><FNAM>][Source: <FILE>@<LINE>] << Text: <TEXT><IS01>";

    char buf[256];

    const unsigned len = gkr_log_apply_text_format(buf, sizeof(buf), fmt, &msg, gkr_log_fo_flag_escape_text_dquote, nullptr, 0, 0);

    const char* result = "[Thread: Main (12345)][Severity: Info][Facility: Testing][Source: main.cpp@16] < Text: Test message";

    CHECK(len == unsigned(std::strlen(result)));
    CHECK(0 == std::strcmp(buf, result));
}

TEST_CASE("logging.logger.format.output. escape_dquote")
{
    char buf[256];

    msg.messageText = "Testing with \"double quote\"!";
    msg.messageLen  = unsigned(std::strlen(msg.messageText));

    const unsigned len = gkr_log_apply_text_format(buf, sizeof(buf), "<TEXT>", &msg, gkr_log_fo_flag_escape_text_dquote, nullptr, 0, 0);

    CHECK(len == msg.messageLen + 2);
    CHECK(0 == std::strcmp(buf, "Testing with \\\"double quote\\\"!"));
}

TEST_CASE("logging.logger.format.output. main")
{
    char buf1[256];
    char buf2[256];

    msg.messageText = "Test message";
    msg.messageLen  = unsigned(std::strlen(msg.messageText));

    constexpr const char* args[] = PLOG_CONSOLE_ARGS_STRS;

    int severities[] = { plog::fatal, plog::info, plog::verbose };

    for(int severity : severities)
    {
        msg.severity = severity;

        unsigned len1 = gkr_log_apply_time_format(
            buf1,
            256,
            PLOG_FULL_MSG_FORMAT,
            msg.stamp,
            0);
        CHECK(len1 > 0);
        REQUIRE(errno == 0);

        unsigned len2 = gkr_log_apply_text_format(
            buf2,
            256,
            buf1,
            &msg,
            gkr_log_fo_flag_use_coloring | gkr_log_fo_flag_use_inserts | gkr_log_fo_flag_use_padding,
            args,
            PLOG_CONSOLE_ARGS_ROWS,
            PLOG_CONSOLE_ARGS_COLS
            );
        CHECK(len2 > 0);
        REQUIRE(errno == 0);

        std::cout << buf2 << std::endl;
    }
    std::cout << "END" << std::endl;
}
