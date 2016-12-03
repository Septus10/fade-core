#pragma once

#include <definitions.hpp>

namespace fade
{

/**
 * Main engine loop
 */
class application abstract
{
public:
	/**
	 * 
	 */
	virtual void tick() = 0;

private:
	
};

std::unique_ptr<application> __declspec(dllexport) get_application();

}