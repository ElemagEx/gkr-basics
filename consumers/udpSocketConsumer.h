#pragma once

#include <gkr/log/consumer.h>

#include <string>

namespace gkr
{
namespace log
{

class udpSocketConsumer : public consumer
{
	udpSocketConsumer() noexcept = delete;

	udpSocketConsumer           (const udpSocketConsumer&) noexcept = delete;
	udpSocketConsumer& operator=(const udpSocketConsumer&) noexcept = delete;

public:
	udpSocketConsumer(udpSocketConsumer&& other) noexcept;
	udpSocketConsumer& operator=(udpSocketConsumer&& other) noexcept;

public:
	udpSocketConsumer(const char* addr, unsigned short port);
	virtual ~udpSocketConsumer();

protected:
	virtual bool init_logging() override;
	virtual void done_logging() override;

	virtual bool filter_log_message(const message& msg) override;

	virtual void consume_log_message(const message& msg) override;

public:
	bool setRemoteAddress(const char* addr, unsigned short port);

private:
	void clearRemoteHostAddress() noexcept;

private:
	std::string m_processName;
	std::string m_hostName;

	struct hostaddr_t { alignas(4) char bytes[32] = {0}; };

	hostaddr_t m_remoteAddr;
};

}
}
