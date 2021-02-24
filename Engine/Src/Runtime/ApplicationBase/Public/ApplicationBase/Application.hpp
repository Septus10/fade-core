#pragma once

// Application base
#include <ApplicationBase/ApplicationBaseApi.hpp>

// Core definitions
#include <Core/Definitions.hpp>
#include <Core/Math/Vector2.hpp>
#include <Core/Containers/SharedPointer.hpp>

// Platformcore definitions
#include <PlatformCore/Application/PlatformContext.hpp>

namespace Fade { inline namespace ApplicationBase {

enum class EInitializationResult : u8
{
	SUCCESS = 0x01,
	FAIL	= 0x02,
};

enum class ETickResult : u8
{
	STOP		= 0x01,
	CONTINUE	= 0x02,
	PAUSE		= 0x04,
};

class IApplication
{
protected:
	/**
	 * protected ctor
	 * this interface should never be instantiated
	 */
	IApplication()
	{
	}

	/**
	 * unique pointer to the platform application class
	 * this class handles most platform specific application functionality, such as creating windows/message handling
	 */
	FADE_APPLICATIONBASE_API static TUniquePtr<PlatformCore::CPlatformContext> sm_PlatformContext;

public:
	/**
	 * virtual dtor
	 */
	virtual ~IApplication() {}

	/**
	 * initialization function
	 */
	virtual EInitializationResult Initialize() { return EInitializationResult::SUCCESS; };

	/**
	 * post initialization function
	 */
	virtual EInitializationResult PostInitialize() { return EInitializationResult::SUCCESS; }

	/**
	 *
	 */
	virtual void Deinitialize() { };

	/**
	 *
	 */
	virtual ETickResult Tick(float a_DeltaTime) { return ETickResult::CONTINUE; }

	/**
	 *
	 */
	virtual ETickResult PostTick(float a_DeltaTime) { return ETickResult::CONTINUE; }
};

} }