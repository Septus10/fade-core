#include <Threading/thread_impl.hpp>

namespace fade { namespace threading { 

void thread_impl::add_task(std::function<void()> task)
{
	task_ = task;
}

std::unique_ptr<thread> threading::get_thread()
{
	return std::make_unique<thread_impl>();
}

} }