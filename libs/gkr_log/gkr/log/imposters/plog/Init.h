#pragma once
#include <plog/Logger.h>

namespace plog
{
template<int instanceId, template<class> class Appender, class Formatter>
inline Logger<instanceId>& init(Severity maxSeverity = none, Appender<Formatter>* appender = nullptr)
{
    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
    return logger.addAppender<Appender, Formatter>(appender);
}
template<int instanceId>
inline Logger<instanceId>& init(Severity maxSeverity = none, IAppender* appender = nullptr)
{
    auto& logger = *impl::getLoggerInstance<instanceId>(maxSeverity);
//  if(appender) logger.addAppender(appender);
    return logger;
}
inline Logger<PLOG_DEFAULT_INSTANCE_ID>& init(Severity maxSeverity = none, IAppender* appender = nullptr)
{
    return init<PLOG_DEFAULT_INSTANCE_ID>(maxSeverity, appender);
}
}
