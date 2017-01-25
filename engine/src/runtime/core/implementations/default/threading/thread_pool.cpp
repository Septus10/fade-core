#include <threading/thread_pool_impl.hpp>

#include <threading/thread_impl.hpp>

#include <thread>

namespace fade { namespace threading { 

thread_pool_impl::thread_pool_impl()
{
	u32 concurrency = std::thread::hardware_concurrency();
	for (u32 i = 0; i < concurrency; i++)
	{
		available_threads_.push_back(std::make_unique<thread_impl>());
	}
}

thread_pool_impl::thread_pool_impl(u32 num_threads)
{
	for (u32 i = 0; i < num_threads; i++)
	{
		available_threads_.push_back(std::make_unique<thread_impl>());
	}
}

thread* thread_pool_impl::get_thread()
{
	if (available_threads_.empty())
	{
		// no threads available
		return nullptr;
	}

	thread* ptr = available_threads_[0].get();
	std::unique_ptr<thread> thread = move(available_threads_[0]);
	available_threads_.erase(available_threads_.begin());
	working_threads_.push_back(move(thread));
	return ptr;
}

std::unique_ptr<thread_pool> threading::get_thread_pool()
{
	return std::make_unique<thread_pool_impl>();
}

std::unique_ptr<thread_pool> threading::get_thread_pool(u32 num_threads)
{
	return std::make_unique<thread_pool_impl>(num_threads);
}

} }