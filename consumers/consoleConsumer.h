#pragma once

#include <gkr/log/consumer.h>

namespace gkr
{
namespace log
{

class consoleConsumer : public consumer
{
	consoleConsumer           (const consoleConsumer&) noexcept = delete;
	consoleConsumer& operator=(const consoleConsumer&) noexcept = delete;

private:
	char*    m_bufferPtr;
	unsigned m_bufferCch;

public:
	consoleConsumer(consoleConsumer&& other) noexcept
		: m_bufferPtr(other.m_bufferPtr)
		, m_bufferCch(other.m_bufferCch)
	{
		other.m_bufferPtr = nullptr;
		other.m_bufferCch = 0;
	}
	consoleConsumer& operator=(consoleConsumer&& other) noexcept
	{
		m_bufferPtr = other.m_bufferPtr;
		m_bufferCch = other.m_bufferCch;

		other.m_bufferPtr = nullptr;
		other.m_bufferCch = 0;
		return *this;
	}

public:
	consoleConsumer(unsigned bufferCch = 2*1024);
	virtual ~consoleConsumer();

protected:
	virtual bool init_logging() override;
	virtual void done_logging() override;

	virtual bool filter_log_message(const message& msg) override;

	virtual void consume_log_message(const message& msg) override;

protected:
	virtual void composeOutput(char* buffer, unsigned cch, const message& msg);
};

}
}
