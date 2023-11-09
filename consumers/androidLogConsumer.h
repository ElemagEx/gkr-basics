#pragma once

#include <gkr/log/consumer.h>

namespace gkr
{
namespace log
{

class androidLogConsumer : public consumer
{
	androidLogConsumer           (const androidLogConsumer&) noexcept = delete;
	androidLogConsumer& operator=(const androidLogConsumer&) noexcept = delete;

private:
	char*    m_bufferPtr;
	unsigned m_bufferCch;

public:
	androidLogConsumer(androidLogConsumer&& other) noexcept
		: m_bufferPtr(other.m_bufferPtr)
		, m_bufferCch(other.m_bufferCch)
	{
		other.m_bufferPtr = nullptr;
		other.m_bufferCch = 0;
	}
	androidLogConsumer& operator=(androidLogConsumer&& other) noexcept
	{
		m_bufferPtr = other.m_bufferPtr;
		m_bufferCch = other.m_bufferCch;

		other.m_bufferPtr = nullptr;
		other.m_bufferCch = 0;
		return *this;
	}

public:
	androidLogConsumer(unsigned bufferCch = 2*1024);
	virtual ~androidLogConsumer();

protected:
	virtual bool init_logging() override;
	virtual void done_logging() override;

	virtual bool filter_log_message(const message& msg) override;

	virtual void consume_log_message(const message& msg) override;

protected:
	virtual int getPriority(const message& msg);

	virtual const char* getTag(const message& msg);

	virtual void formatText(char* buffer, unsigned cch, const message& msg);
};

}
}
