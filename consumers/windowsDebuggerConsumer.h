#pragma once

#include <gkr/log/consumer.h>

namespace gkr
{
namespace log
{

class windowsDebuggerConsumer : public consumer
{
	windowsDebuggerConsumer           (const windowsDebuggerConsumer&) noexcept = delete;
	windowsDebuggerConsumer& operator=(const windowsDebuggerConsumer&) noexcept = delete;

private:
	char*    m_bufferPtr;
	unsigned m_bufferCch;

public:
	windowsDebuggerConsumer(windowsDebuggerConsumer&& other) noexcept
		: m_bufferPtr(other.m_bufferPtr)
		, m_bufferCch(other.m_bufferCch)
	{
		other.m_bufferPtr = nullptr;
		other.m_bufferCch = 0;
	}
	windowsDebuggerConsumer& operator=(windowsDebuggerConsumer&& other) noexcept
	{
		m_bufferPtr = other.m_bufferPtr;
		m_bufferCch = other.m_bufferCch;

		other.m_bufferPtr = nullptr;
		other.m_bufferCch = 0;
		return *this;
	}

public:
	windowsDebuggerConsumer(unsigned bufferCch = 2*1024);
	virtual ~windowsDebuggerConsumer();

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
