#pragma once

#include <PlatformCore/PlatformCoreApi.hpp>
#include <PlatformCore/Application/PlatformContext.hpp>

#include <Window/WindowsWindow.hpp>

#include <Core/Definitions.hpp>

#include <Windows.h>

namespace Fade { inline namespace PlatformCore {

namespace Windows {
	extern "C" FADE_PLATFORMCORE_API HINSTANCE g_InstanceHandle;
}

class CWindowsContext : public CPlatformContext
{
protected:
	static const TCHAR sm_AppWindowClass[];

	static LRESULT CALLBACK AppWndProc(HWND a_WindowHandle, u32 a_Msg, WPARAM a_wParam, LPARAM a_lParam);

public:
	static CWindowsContext* CreateWindowsContext(HINSTANCE a_InstanceHandle, HICON a_IconHandle);

	bool RegisterWindowClass(const HINSTANCE a_InstanceHandle, const HICON a_IconHandle);

	u32 ProcessMessage(HWND a_WindowHandle, u32 a_Msg, WPARAM a_wParam, LPARAM a_lParam);

	TSharedPtr<CWindowsWindow> GetWindowFromHwnd(HWND a_WindowHandle);

	TSharedPtr<CWindow> GetWindowObject() override;
private:
	CWindowsContext(const HINSTANCE a_InstanceHandle , const HICON a_IconHandle);

	HINSTANCE m_InstanceHandle;
};

} }