#pragma once

#include <gkr/log/consumer.h>
#include <gkr/container/raw_buffer.h>

namespace gkr
{

class windowsDebuggerConsumer : public log::consumer
{
	windowsDebuggerConsumer           (const windowsDebuggerConsumer&) noexcept = delete;
	windowsDebuggerConsumer& operator=(const windowsDebuggerConsumer&) noexcept = delete;

private:
	using raw_buffer_t = raw_buffer<>;

	raw_buffer_t m_buffer;

public:
	windowsDebuggerConsumer(windowsDebuggerConsumer&& other) noexcept(
		std::is_nothrow_move_constructible<raw_buffer_t>::value
		)
		: m_buffer(std::move(other.m_buffer))
	{
	}
	windowsDebuggerConsumer& operator=(windowsDebuggerConsumer&& other) noexcept(
		std::is_nothrow_move_assignable<raw_buffer_t>::value
		)
	{
		m_buffer = std::move(other.m_buffer);
		return *this;
	}

public:
	windowsDebuggerConsumer(std::size_t bufferCapacity = 2*1024);
	virtual ~windowsDebuggerConsumer() override;

protected:
	virtual bool init_logging() override;
	virtual void done_logging() override;

	virtual bool filter_log_message(const log::message& msg) override;

	virtual void consume_log_message(const log::message& msg) override;

protected:
	virtual void composeOutput(char* buffer, std::size_t cch, const log::message& msg);
};

}
