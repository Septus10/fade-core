#include <Threading/thread_pool.hpp>
#include <Threading/thread.hpp>

#include <thread>

using namespace fade;
using namespace threading;

class thread_pool::impl
{
public:
	std::vector<std::unique_ptr<thread>> available_threads_;
	std::vector<std::unique_ptr<thread>> working_threads_;
};

thread_pool::thread_pool()
{
	u32 concurrency = std::thread::hardware_concurrency();
	for (u32 i = 0; i < concurrency; i++)
	{
		impl_->available_threads_.push_back(std::make_unique<thread>());
	}
}

thread_pool::thread_pool(u32 num_threads)
{
	for (u32 i = 0; i < num_threads; i++)
	{
		impl_->available_threads_.push_back(std::make_unique<thread>());
	}
}

thread_pool::~thread_pool()
{
	
}

thread* thread_pool::get_thread()
{
	if (impl_->available_threads_.empty())
	{
		// no threads available
		return nullptr;
	}

	thread* ptr = impl_->available_threads_[0].get();
	std::unique_ptr<thread> thread = move(impl_->available_threads_[0]);
	impl_->available_threads_.erase(impl_->available_threads_.begin());
	impl_->working_threads_.push_back(move(thread));
	return ptr;
}



