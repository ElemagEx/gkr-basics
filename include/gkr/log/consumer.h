#pragma once

namespace gkr
{
namespace log
{

struct message;

class consumer
{
public:
	virtual ~consumer() = default;

	virtual bool init_logging() = 0;
	virtual void done_logging() = 0;

	virtual void consume_log_message(const message& msg) = 0;
};

}
}
