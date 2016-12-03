#pragma once

#include <function.hpp>
#include <pimpl.hpp>

namespace fade
{ 
	
namespace threading
{

class thread
{
public:
	thread();
	~thread();

	void add_task(fade::function task);

private:
	FADE_MAKE_PIMPL
};


}	

}