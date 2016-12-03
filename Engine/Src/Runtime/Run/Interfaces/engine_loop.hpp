#pragma once

#include <definitions.hpp>
#include <application.hpp>

#include <memory>

namespace fade
{

int main();

/**
 * Main engine loop
 */
class engine_loop
{
public:
	/**
	 *
	 */
	engine_loop();

	/**
	 *
	 */	
	~engine_loop();

	/**
	 * 
	 */
	i32 pre_initialize();

	/**
	 * 
	 */
	i32 initialize();

	/**
	* 
	*/
	i32 post_initialize();

	/**
	* 
	*/
	i32 deinitialize();

	/**
	 * 
	 */
	virtual void tick();

	/**
	 *
	 */
	void should_close(bool should_close);

	/**
	 *
	 */
	bool should_close() const;
private:
	std::unique_ptr<application> app_;
	bool run_;
};

}