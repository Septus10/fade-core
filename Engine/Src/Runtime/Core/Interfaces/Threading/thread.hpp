#pragma once

#include <core_api.hpp>
#include <function.hpp>
#include <pimpl.hpp>

namespace fade
{ 
	
namespace threading
{

class FADE_API thread abstract
{
public:
	virtual ~thread() = default;

	virtual void add_task(std::function<void()> task) = 0;
};


std::unique_ptr<thread> FADE_API get_thread();

}	

}