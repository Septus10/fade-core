#pragma once

#include <core/threading/thread_pool.hpp>

#include <core/threading/thread.hpp>

namespace fade
{
	
namespace threading
{
	
class thread_pool_impl : public thread_pool
{
public:
	thread_pool_impl();
	thread_pool_impl(u32 num_threads);

	~thread_pool_impl() = default;

	thread* get_thread() override;

private:
	std::vector<std::unique_ptr<thread>> available_threads_;
	std::vector<std::unique_ptr<thread>> working_threads_;

};

}

}