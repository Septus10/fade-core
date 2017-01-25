#pragma once

#include <core/threading/thread.hpp>

namespace fade
{
	
namespace threading
{
	
class thread_impl : public thread
{
public:
	~thread_impl() = default;

	void add_task(std::function<void()> task) override;

private:
	std::function<void()> task_;
};

}

}