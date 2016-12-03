#include <Threading/thread.hpp>

using namespace fade;
using namespace threading;

class thread::impl
{
public:
	impl() : task_(nullptr)
	{}
	fade::function task_;

};

thread::thread()
{
	impl_ = std::make_unique<thread::impl>();
}

thread::~thread()
{
	
}


void thread::add_task(fade::function task)
{
	impl_->task_ = task;
}

