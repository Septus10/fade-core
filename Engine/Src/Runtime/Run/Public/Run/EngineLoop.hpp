#pragma once

#include <Application/Application.hpp>

#include <Core/definitions.hpp>
#include <Core/Pimpl.hpp>
#include <Core/Timer.hpp>
#include <core/fstl/memory.hpp>
#include <core/fstl/vector.hpp>

namespace Fade
{

/**
 * Main engine loop class
 */
class CEngineLoop
{
public:
	/**
	 * CTor
	 */
	CEngineLoop();

	/**
	 * DTor
	 */	
	~CEngineLoop();

	/**
	 * Pre initialization, mandatory resources will be loaded
	 */
	i32 PreInitialize();

	/**
	 * Initialization, window creation and module initialization will happen here
	 */
	i32 Initialize();

	/**
	* Post initialization, resources that depend on everything else being initialized will be initialized here
	*/
	i32 PostInitialize();

	/**
	* Deinitialized all resources
	*/
	i32 DeInitialize();

	/**
	 * Engine main tick function
	 */
	void Tick();

	/**
	 * Sets whether we should stop the application
	 */
	void ShouldStop(bool a_ShouldStop);

	/**
	 * Gets whether we should stop the application
	 */
	bool ShouldStop() const;

	/**
	 * Reads module config file and loads those modules
	 */
	void LoadModules();

	/**
	 * Unloads all modules
	 */
	void UnloadModules();

private:
	CTimer m_Timer;
};

}
