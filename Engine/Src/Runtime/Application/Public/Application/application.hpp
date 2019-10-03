#pragma once

#include <Application/ApplicationApi.hpp>
#include <Core/Definitions.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <Core/Containers/NTree.hpp>
#include <PlatformCore/Window/Window.hpp>

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

	virtual bool PreInitialize() = 0;

	virtual bool Initialize() = 0;

	virtual bool PostInitialize() = 0;

	virtual bool DeInitialize() = 0;

	virtual const PlatformCore::CWindow* GetMainWindowPtr() const { return nullptr; }

	//u32 CreateWindow(i32 a_ParentIndex = -1);
	//
	//bool DestroyWindow(u32 a_WindowIndex);

protected:
};


/**
 * Application factory function
 */
Fade::TUniquePtr<CApplication> FADE_APPLICATION_API GetApplication();

}
