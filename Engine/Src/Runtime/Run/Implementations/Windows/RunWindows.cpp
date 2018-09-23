#include <Windows.h>

#include <Application/Application.hpp>
#include <PlatformCore/PlatformCore.hpp>
#include <PlatformCore/Window/WindowManager.hpp>
#include <Core/Containers/SmartPointers.hpp>

bool g_ShouldQuit = false;
Fade::CUniquePtr<Fade::CApplication> g_Application;

LRESULT CALLBACK WindowProcedure(HWND a_Window, UINT a_Message, WPARAM a_wParam, LPARAM a_lParam)
{
	switch (a_Message)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		Fade::GetServiceLocator().GetService<Fade::PlatformCore::CWindowManager>()->OnWindowDestroyed(static_cast<void*>(a_Window));
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
	// Create an application
	g_Application = Fade::GetApplication();

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

	g_Application->PreInitialize();

	g_Application->Initialize();

	g_Application->PostInitialize();

	MSG Message;
	while (!(g_Application->Tick(0.f) == Fade::ETickResult::STOP))
	{
		while (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}

	g_Application->DeInitialize();

	return 0;
}