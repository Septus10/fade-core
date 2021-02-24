#pragma once

#include <PlatformCore/PlatformCoreApi.hpp>
#include <PlatformCore/Application/PlatformMessageHandler.hpp>

#include <Core/Containers/Array.hpp>
#include <Core/Containers/SharedPointer.hpp>
#include <Core/Containers/UniquePointer.hpp>

namespace Fade { inline namespace PlatformCore {

class CWindow;

/**
 * Platform application base class.
 * inspired by Unreal Engine, https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/ApplicationCore/Public/GenericPlatform/GenericApplication.h
 */
class CPlatformContext
{
protected:
	FADE_PLATFORMCORE_API static CPlatformContext* sm_PlatformContext;

public:
	template <typename PlatformContextClass = CPlatformContext>
	static PlatformContextClass* GetPlatformContext()
	{
		return (PlatformContextClass*)(sm_PlatformContext);
	}

	virtual void SetMessageHandler(IPlatformMessageHandler* a_MessageHandler)
	{
		m_MessageHandler = TSharedPtr<IPlatformMessageHandler>(a_MessageHandler);
	}

	//virtual void ProcessMessages();
	//virtual void Tick(const float a_DeltaTime);

	virtual void RequestQuit(bool a_NewQuit = true) { m_QuitRequested = a_NewQuit; }
	virtual bool IsQuitRequested() { return m_QuitRequested; }

	virtual TSharedPtr<CWindow> GetWindowObject() { return nullptr;}

protected:
	TSharedPtr<IPlatformMessageHandler> m_MessageHandler;
	TArray<TSharedPtr<CWindow>> m_PlatformWindows;

	bool m_QuitRequested = false;
};

} }