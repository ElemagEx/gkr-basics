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
	udpSocketConsumer(const char* addr, unsigned short port, unsigned maxPacketSize, unsigned bufferSize = 2*1024);
	virtual ~udpSocketConsumer() override;

protected:
	virtual bool init_logging() override;
	virtual void done_logging() override;

	virtual bool filter_log_message(const message& msg) override;

	virtual void consume_log_message(const message& msg) override;

public:
	bool setRemoteAddress(const char* addr, unsigned short port);

private:
	void clearRemoteAddress() noexcept;

	bool retrieveProcessName();
	bool retrieveHostName();

private:
	std::string m_processName;
	std::string m_hostName;

	struct hostaddr_t { alignas(4) char bytes[32] = {0}; };

	hostaddr_t m_remoteAddr;

	unsigned   m_maxPacketSize = 0;
	int        m_pid           = 0;
};

}
}
