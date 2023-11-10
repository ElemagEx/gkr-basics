#pragma once

#include <gkr/log/consumer.h>
#include <gkr/container/raw_buffer.h>

namespace gkr
{
namespace log
{

class androidLogConsumer : public consumer
{
	androidLogConsumer           (const androidLogConsumer&) noexcept = delete;
	androidLogConsumer& operator=(const androidLogConsumer&) noexcept = delete;

private:
	using raw_buffer_t = raw_buffer<>;

	raw_buffer_t m_buffer;

public:
	androidLogConsumer(androidLogConsumer&& other) noexcept(
		std::is_nothrow_move_constructible<raw_buffer_t>::value
		)
		: m_buffer(std::move(other.m_buffer))
	{
	}
	androidLogConsumer& operator=(androidLogConsumer&& other) noexcept(
		std::is_nothrow_move_assignable<raw_buffer_t>::value
		)
	{
		m_buffer = std::move(other.m_buffer);
		return *this;
	}

public:
	androidLogConsumer(std::size_t bufferInitialCapacity = 2*1024);
	virtual ~androidLogConsumer() override;

protected:
	virtual bool init_logging() override;
	virtual void done_logging() override;

	virtual bool filter_log_message(const message& msg) override;

	virtual void consume_log_message(const message& msg) override;

protected:
	virtual int getPriority(const message& msg);

	virtual const char* getTag(const message& msg);

	virtual void formatText(char* buffer, std::size_t cch, const message& msg);
};

}
}
