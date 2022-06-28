#include <memory>

namespace gkr
{
class objects_waiter;
extern
thread_local std::shared_ptr<objects_waiter> g_this_thread_objects_waiter;
thread_local std::shared_ptr<objects_waiter> g_this_thread_objects_waiter = nullptr;
}
