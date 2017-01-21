#pragma once

#include <core/core_api.hpp>
#include <definitions.hpp>
#include <pimpl.hpp>

#include <vector>

namespace fade
{ 
	
namespace threading
{
	
class thread;
class FADE_API thread_pool abstract
{
public:
	virtual ~thread_pool() = default;

	virtual thread* get_thread() = 0;
};

std::unique_ptr<thread_pool> FADE_API get_thread_pool();
std::unique_ptr<thread_pool> FADE_API get_thread_pool(u32 num_threads);

}	

}