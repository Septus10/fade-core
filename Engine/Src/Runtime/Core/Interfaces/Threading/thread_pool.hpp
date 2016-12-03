#pragma once

#include <definitions.hpp>
#include <pimpl.hpp>

#include <vector>

namespace fade
{ 
	
namespace threading
{
	
class thread;
class thread_pool
{
public:
	thread_pool();
	thread_pool(u32 num_threads);
	~thread_pool();

	thread* get_thread();

private:
	FADE_MAKE_PIMPL

};

}	

}