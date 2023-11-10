#pragma once

#include <gkr/log/consumer.h>
#include <gkr/container/raw_buffer.h>

namespace gkr
{
namespace log
{

class consoleConsumer : public consumer
{
	consoleConsumer           (const consoleConsumer&) noexcept = delete;
	consoleConsumer& operator=(const consoleConsumer&) noexcept = delete;

private:
	using raw_buffer_t = raw_buffer<>;

	raw_buffer_t m_buffer;

public:
	consoleConsumer(consoleConsumer&& other) noexcept(
		std::is_nothrow_move_constructible<raw_buffer_t>::value
		)
		: m_buffer(std::move(other.m_buffer))
	{
	}
	consoleConsumer& operator=(consoleConsumer&& other) noexcept(
		std::is_nothrow_move_assignable<raw_buffer_t>::value
		)
	{
		m_buffer = std::move(other.m_buffer);
		return *this;
	}

public:
	consoleConsumer(std::size_t bufferInitialCapacity = 2*1024);
	virtual ~consoleConsumer() override;

protected:
	virtual bool init_logging() override;
	virtual void done_logging() override;

	virtual bool filter_log_message(const message& msg) override;

	virtual void consume_log_message(const message& msg) override;

protected:
	virtual void composeOutput(char* buffer, std::size_t cch, const message& msg);
};

}
}
