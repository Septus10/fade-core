#define _CRT_SECURE_NO_WARNINGS 1

#include <Windows.h>

#include <Application/Application.hpp>
#include <PlatformCore/PlatformCore.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <Core/Timer.hpp>
#include <PlatformCore/Window/Window.hpp>
#include <iostream>

bool g_ShouldQuit = false;
Fade::TUniquePtr<Fade::CApplication> g_Application;

LRESULT CALLBACK WindowProcedure(HWND a_Window, UINT a_Message, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_Message)
	{
	case WM_CREATE:
		DragAcceptFiles(a_Window, true);
		break;
	case WM_DROPFILES:
	{
		HDROP hDropInfo = (HDROP)a_wParam;
		static const UINT buffSize = 512;
		char buf[512];
		DragQueryFile(hDropInfo, 0, buf, buffSize);
		std::cout << "Drop file: " << buf << "\n";
		break;
	}
	case WM_DESTROY:
	case WM_CLOSE:
		if (g_Application->GetMainWindowPtr() != nullptr &&
			g_Application->GetMainWindowPtr()->GetWindowHandle() == a_Window)
		{
			g_ShouldQuit = true;
		}
		break;
	case WM_KEYDOWN:
		g_Application->OnKeyDown(a_wParam);
		break;
	case WM_KEYUP:
		g_Application->OnKeyUp(a_wParam);
		break;
	case WM_CHAR:
		break;
	default:
		return DefWindowProc(a_Window, a_Message, a_wParam, a_lParam);
		break;
	}

	return 0;
}

int WINAPI WinMain(
	HINSTANCE a_InstanceHandle,
	HINSTANCE a_PreviousInstanceHandle,
	LPSTR a_CommandLine,
	int a_CommandShow)
{
#ifdef _DEBUG
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
	std::cout << "Console created.\n";
#endif

	// Create an application
	g_Application = Fade::GetApplication();
	std::cout << "Application created.\n";

	// Register the window class
	WNDCLASSEX WindowClass = { 0 };
	WindowClass.cbSize			= sizeof(WNDCLASSEX);
	WindowClass.lpfnWndProc		= WindowProcedure;
	WindowClass.cbClsExtra		= 0;
	WindowClass.cbWndExtra		= 0;
	WindowClass.hInstance		= a_InstanceHandle;
	WindowClass.hIcon			= LoadIcon(a_InstanceHandle, MAKEINTRESOURCE(IDI_APPLICATION));
	WindowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	WindowClass.lpszMenuName	= NULL;
	WindowClass.lpszClassName	= Fade::PlatformCore::sg_WindowClassName;
	WindowClass.hIconSm			= LoadIcon(a_InstanceHandle, MAKEINTRESOURCE(IDI_APPLICATION));
	WindowClass.style			= CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&WindowClass))
	{
		DWORD ErrorID = ::GetLastError();
		if (ErrorID == 0)
		{
			MessageBox(NULL, TEXT("Unable to register window class"), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			MessageBox(nullptr, Fade::PlatformCore::GetLastErrorMessage(), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
		}
		
		return 1;
	}

	std::cout << "Window class created.\n";

	if (!g_Application->PreInitialize() ||
		!g_Application->Initialize()	||
		!g_Application->PostInitialize())
	{
		std::cout << "Press any key to continue\n";
		std::cin.get();
		return 1;
	}

	const Fade::u32 DesiredFPS = 120;
	const Fade::f64 FrameTime = 1.0 / (double)DesiredFPS;
	Fade::f64 CurrentTime = FrameTime;

	MSG Message;
	Fade::CTimer TickTimer;
	TickTimer.Start();
	bool InitialTick = true;
	while (!g_ShouldQuit)
	{		
		CurrentTime = TickTimer.Elapsed();
		if (CurrentTime >= FrameTime || InitialTick)
		{
			TickTimer.Reset();
			InitialTick = false;
			if (g_Application->Tick(CurrentTime) != Fade::ETickResult::CONTINUE)
			{
				g_ShouldQuit = true;
			}
		}		
		while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	g_Application->DeInitialize();

	return 0;
}