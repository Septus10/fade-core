#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>
#include <windowsx.h>

#include <ApplicationBase//Application.hpp>
#include <Application/ApplicationFactory.hpp>
#include <PlatformCore/PlatformCore.hpp>
#include <Core/Log.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <Core/Timer.hpp>
#include <Core/Math/Vector2.hpp>
#include <PlatformCore/Window/Window.hpp>
#include <iostream>

#include <Application/WindowsApplication.hpp>

int WINAPI WinMain(
	HINSTANCE a_InstanceHandle,
	HINSTANCE a_PreviousInstanceHandle,
	LPSTR a_CommandLine,
	int a_CommandShow)
{	
	// Instantiate the logger on the stack, then save a static pointer for the public getter function
	Fade::CCompositeLogger CompositeLogger;
	CompositeLogger.RegisterLogger(new Fade::CFileLogger("./Intermediate/Logs/Log.txt"));

	// Temporary creation of console window
#ifdef FADE_DEBUG 
	if (!AllocConsole())
	{
		DWORD ErrorID = ::GetLastError();
		if (ErrorID == 0)
		{
			MessageBox(NULL, TEXT("Unable to allocate console"), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			MessageBox(nullptr, Fade::PlatformCore::GetLastErrorMessage(), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		}

		return 1;
	}

	freopen("conin$","r",stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);

	CompositeLogger.RegisterLogger(new Fade::CStreamLogger(&std::cout));
	FADE_LOG(Info, Run, "Console created.");
#endif	

	Fade::PlatformCore::Windows::g_InstanceHandle = a_InstanceHandle;

	bool g_ShouldQuit = false;

	// Create an application
	Fade::TUniquePtr<Fade::IApplication> g_Application = Fade::GetApplication();
	FADE_LOG(Info, Run, "Application created.");

	if (g_Application->Initialize()		!= Fade::EInitializationResult::SUCCESS	||
		g_Application->PostInitialize()	!= Fade::EInitializationResult::SUCCESS)
	{
		return 1;
	}

	const Fade::u32 DesiredFPS = 120;
	const float FrameTime = 1.0f / (float)DesiredFPS;
	float CurrentTime = FrameTime;

	MSG Message;
	Fade::CTimer TickTimer;
	TickTimer.Start();
	while (!g_ShouldQuit)
	{		
		if (CurrentTime >= FrameTime)
		{
			TickTimer.Reset();
			g_ShouldQuit = 
				g_Application->Tick(CurrentTime) != Fade::ETickResult::CONTINUE || 
				g_Application->PostTick(CurrentTime) != Fade::ETickResult::CONTINUE;
		}

		while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		CurrentTime = TickTimer.Elapsed<float>();
	}

	return 0;
}