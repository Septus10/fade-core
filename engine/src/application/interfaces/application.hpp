#pragma once

#include <application_api.hpp>
#include <definitions.hpp>

namespace fade
{

/**
 * Engine application
 */
class FADE_API application abstract
{
public:
	/**
	 * Virtual DTor
	 */
	virtual ~application() = default;

	/**
	 * Engine application main tick function
	 * @param delta_time delta time for main tick function
	 */
	virtual void tick(double delta_time) = 0;

	/**
	 * Engine application fixed tick function
	 * @param fixed_delta_time delta time for the fixed tick function
	 */
	virtual void fixed_tick(double fixed_delta_time) = 0;

	virtual void pre_initialize() = 0;

	virtual void initialize() = 0;

	virtual void post_initialize() = 0;

	virtual void deinitialize() = 0;
};


/**
 * Application factory function
 */
std::unique_ptr<application> FADE_API get_application();

}
