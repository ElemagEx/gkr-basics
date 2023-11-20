#include <gkr/log/consumers/text_file_consumer.h>

#include <gkr/diagnostics.h>
#include <gkr/log/message.h>
#include <gkr/sys/process_name.h>
#include <gkr/stamp.h>

#include <cstdio>
#include <cstdlib>

static bool setEoln(int eoln, char* buf)
{
    buf[1] = buf[2] = 0;

    switch(eoln)
    {
        default: buf[0] = 0; return false;

        case gkr_log_textFileEoln_LF  : buf[0] = '\n'; break;
        case gkr_log_textFileEoln_CR  : buf[0] = '\r'; break;
        case gkr_log_textFileEoln_CRLF: buf[0] = '\r'; buf[1] = '\n'; break;
    }
    return true;
}
static void outputToTextFile(std::FILE* file, const char* text, unsigned len, const char* eoln, unsigned cch)
{
    std::fwrite(text, sizeof(char), len, file);
    std::fwrite(eoln, sizeof(char), cch, file);
}

extern "C" {

unsigned gkr_log_textFile_makeFilePath(char* buf, unsigned cch)
{
    char path[260];
    gkr::sys::get_current_process_path(path, 260);

    struct tm tm;
    gkr::stamp_decompose(true, gkr::stamp_now(), tm);

    const int len = std::snprintf(
        buf,
        cch,
        "%s_%04d%02d%02d_%02d%02d%02d.txt",
        path,
        tm.tm_year + 1900,
        tm.tm_mon  + 1,
        tm.tm_mday,
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec
        );
    return unsigned(len);
}

unsigned gkr_log_textFile_ComposeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg)
{
    struct tm tm;
    int ns = gkr::stamp_decompose(true, msg->stamp, tm);

    const int len = std::snprintf(
        buf,
        cch,
        "[%02d:%02d:%02d.%03d][%s][%s][%s] - %s",
        tm.tm_hour,
        tm.tm_min,
        tm.tm_sec,
        ns / 1000000U,
        msg->severityName,
        msg->facilityName,
        msg->threadName,
        msg->messageText
        );
    return unsigned(len);
}

struct data_t
{
    unsigned   (*make_file_path)(char*, unsigned);
    unsigned   (*compose_output)(char*, unsigned, const struct gkr_log_message*);
    std::FILE* file;
    char       eoln[4];
    unsigned   cch;
    char       buf[1];
};

void* gkr_log_textFile_CreateConsumerParam(
    int eoln,
    unsigned buffer_capacity,
    unsigned (*make_file_path)(char*, unsigned),
    unsigned (*compose_output)(char*, unsigned, const struct gkr_log_message*)
    )
{
    Check_Arg_IsValid(buffer_capacity > 0, nullptr);
    Check_Arg_NotNull(make_file_path, nullptr);
    Check_Arg_NotNull(compose_output, nullptr);

    data_t* data = static_cast<data_t*>(std::malloc(sizeof(data_t) + buffer_capacity - 1));

    if(data != nullptr)
    {
        data->make_file_path = make_file_path;
        data->compose_output = compose_output;
        data->file           = nullptr;
        data->cch            = buffer_capacity;

        if(!setEoln(eoln, data->eoln))
        {
            Check_Arg_Invalid(eoln, nullptr);
        }
    }
    return data;
}

int gkr_log_textFile_InitLogging(void* param)
{
    if(param == nullptr) return false;

    char buf[260];
    gkr_log_textFile_makeFilePath(buf, 260);

    data_t* data = static_cast<data_t*>(param);

    data->file = std::fopen(buf, "wb");

    if(data->file == nullptr)
    {
        Check_Failure(false);
    }
    return true;
}

void gkr_log_textFile_DoneLogging(void* param)
{
    if(param == nullptr) return;

    data_t* data = static_cast<data_t*>(param);

    if(data->file != nullptr)
    {
        std::fclose(data->file);
        data->file = nullptr;
    }
    std::free(param);
}

int gkr_log_textFile_FilterLogMessage(void* param, const struct gkr_log_message* msg)
{
    return false;
}

void gkr_log_textFile_ConsumeLogMessage(void* param, const struct gkr_log_message* msg)
{
    data_t* data = static_cast<data_t*>(param);

    const unsigned len = (*data->compose_output)(data->buf, data->cch, msg);

    data->buf[data->cch - 1] = 0;

    const unsigned cch = (data->eoln[1] == 0) ? 1 : 2;

    outputToTextFile(data->file, data->buf, len, data->eoln, cch);
}

}

namespace gkr
{
namespace log
{

text_file_consumer::text_file_consumer(int eoln, unsigned buffer_capacity)
    : m_buf(nullptr)
    , m_cch(buffer_capacity)
    , m_eoln(0)
    , m_file(nullptr)
{
    Check_Arg_IsValid(buffer_capacity > 0, );

    m_buf = new char[buffer_capacity];

    if(!setEoln(eoln, reinterpret_cast<char*>(&m_eoln)))
    {
        Check_Arg_Invalid(eoln, );
    }
}

text_file_consumer::~text_file_consumer()
{
    close_file();

    if(m_buf != nullptr)
    {
        delete [] m_buf;
    }
}

bool text_file_consumer::init_logging()
{
    if(m_eoln == 0) return false;

    if(m_buf == nullptr) return false;

    char buf[260];
    gkr_log_textFile_makeFilePath(buf, 260);

    m_file = std::fopen(buf, "wb");

    return (m_file != nullptr);
}

void text_file_consumer::done_logging()
{
    close_file();
}

bool text_file_consumer::filter_log_message(const message& msg)
{
    return false;
}

void text_file_consumer::consume_log_message(const message& msg)
{
    const unsigned len = compose_output(m_buf, m_cch, msg);

    m_buf[m_cch - 1] = 0;

    const unsigned cch = (reinterpret_cast<char*>(&m_eoln)[1] == 0) ? 1 : 2;
    outputToTextFile(static_cast<std::FILE*>(m_file), m_buf, len, reinterpret_cast<char*>(&m_eoln), cch);
}

unsigned text_file_consumer::make_file_path(char* buf, unsigned cch)
{
    return gkr_log_textFile_makeFilePath(buf, cch);
}

unsigned text_file_consumer::compose_output(char* buf, unsigned cch, const message& msg)
{
    return gkr_log_textFile_ComposeOutput(m_buf, m_cch, &msg);
}

void text_file_consumer::close_file()
{
    if(m_file != nullptr)
    {
        std::fclose(static_cast<std::FILE*>(m_file));
        m_file = nullptr;
    }
}

}
}
