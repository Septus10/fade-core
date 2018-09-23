#pragma once

#include <Application/ApplicationApi.hpp>
#include <Core/Containers/SmartPointers.hpp>
#include <Core/Definitions.hpp>

namespace Fade
{

enum class ETickResult : u8
{
	STOP		= 0x01,
	CONTINUE	= 0x02,
};

/**
 * Engine application
 */
class FADE_APPLICATION_API CApplication
{
public:
	/**
	 * Virtual DTor
	 */
	virtual ~CApplication() = default;

	/**
	 * Engine application main tick function
	 * @param a_DeltaTime delta time for main tick function
	 */
	virtual ETickResult Tick(double a_DeltaTime) = 0;

	/**
	 * Engine application fixed tick function
	 * @param a_FixedDeltaTime delta time for the fixed tick function
	 */
	virtual void FixedTick(double a_FixedDeltaTime) = 0;

	virtual void PreInitialize() = 0;

	virtual void Initialize() = 0;

	virtual void PostInitialize() = 0;

	virtual void DeInitialize() = 0;
};


/**
 * Application factory function
 */
Fade::CUniquePtr<CApplication> FADE_APPLICATION_API GetApplication();

}
