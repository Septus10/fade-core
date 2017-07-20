#pragma once

#include <application/application.hpp>

#include <core/definitions.hpp>
#include <core/pimpl.hpp>
#include <core/timer.hpp>
#include <core/fstl/memory.hpp>
#include <core/fstl/vector.hpp>

#ifdef FADE_PLATFORM_WINDOWS
#include <windows.h>

typedef HMODULE module_handle;

#elif FADE_PLATFORM_LINUX


#elif FADE_PLATFORM_MAC


#endif

namespace fade
{

#ifdef FADE_PLATFORM_WINDOWS
int main(const char* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, fade::u32 nCmdShow);
#else
int main(const char* CmdLine);
#endif

/**
 * Main engine loop class
 */
class engine_loop
{
public:
	/**
	 * CTor
	 */
	engine_loop();

	/**
	 * DTor
	 */	
	~engine_loop();

	/**
	 * Pre initialization, mandatory resources will be loaded
	 */
	i32 pre_initialize();

	/**
	 * Initialization, window creation and module initialization will happen here
	 */
	i32 initialize();

	/**
	* Post initialization, resources that depend on everything else being initialized will be initialized here
	*/
	i32 post_initialize();

	/**
	* Deinitialized all resources
	*/
	i32 deinitialize();

	/**
	 * Engine main tick function
	 */
	void tick();

	/**
	 * Sets whether we should stop the application
	 */
	void should_stop(bool should_stop);

	/**
	 * Gets whether we should stop the application
	 */
	bool should_stop() const;

	/**
	 * Reads module config file and loads those modules
	 */
	void load_modules();

	/**
	 * Unloads all modules
	 */
	void unload_modules();

private:
	fstl::vector<module_handle> module_list_;
	fstl::unique_ptr<application> app_;
	std::unique_ptr<timer> timer_;
	bool run_;
};

}
