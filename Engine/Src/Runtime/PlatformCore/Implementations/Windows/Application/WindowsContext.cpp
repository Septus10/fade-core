#include <Application/WindowsContext.hpp>

#include <PlatformCore/PlatformCore.hpp>
#include <Core/Log.hpp>

#include <Window/WindowsWindow.hpp>

#include <windowsx.h>

namespace Fade { inline namespace PlatformCore {

extern "C"
{
	FADE_PLATFORMCORE_API HINSTANCE Windows::g_InstanceHandle = NULL;
}

const TCHAR CWindowsContext::sm_AppWindowClass[] = TEXT("FadeWindow");


CWindowsContext * CWindowsContext::CreateWindowsContext(HINSTANCE a_InstanceHandle, HICON a_IconHandle)
{
	sm_PlatformContext = new CWindowsContext(a_InstanceHandle, a_IconHandle);
	return GetPlatformContext<CWindowsContext>();
}

bool CWindowsContext::RegisterWindowClass(const HINSTANCE a_InstanceHandle, const HICON a_IconHandle)
{
	// Register the window class
	WNDCLASSEX WindowClass = { 0 };
	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.lpfnWndProc = AppWndProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = a_InstanceHandle;
	WindowClass.hIcon = a_IconHandle;
	WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WindowClass.lpszMenuName = NULL;
	WindowClass.lpszClassName = sm_AppWindowClass;
	WindowClass.hIconSm = a_IconHandle;
	WindowClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!::RegisterClassEx(&WindowClass))
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

		return false;
	}

	return true;
}

LRESULT CWindowsContext::AppWndProc(HWND a_WindowHandle, u32 a_Msg, WPARAM a_wParam, LPARAM a_lParam)
{
	return GetPlatformContext<CWindowsContext>()->ProcessMessage(a_WindowHandle, a_Msg, a_wParam, a_lParam);
}

static i32 s_PreviousX = -1;
static i32 s_PreviousY = -1;

u32 CWindowsContext::ProcessMessage(HWND a_WindowHandle, u32 a_Msg, WPARAM a_wParam, LPARAM a_lParam)
{
	TSharedPtr<CWindowsWindow> Window = GetWindowFromHwnd(a_WindowHandle);
	switch (a_Msg)
	{
	case WM_CLOSE:	
		// handle window closing
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		break;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		break;
	case WM_CHAR:
		break;
		// Mouse Button Down
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_XBUTTONDBLCLK:
	case WM_XBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
	case WM_XBUTTONUP:
	{
		if (!m_MessageHandler)
		{
			break;
		}

		POINT CursorPoint;
		CursorPoint.x = GET_X_LPARAM(a_lParam);
		CursorPoint.y = GET_Y_LPARAM(a_lParam);

		ClientToScreen(a_WindowHandle, &CursorPoint);

		EMouseButton Button = EMouseButton::None;
		bool bDoubleClick = false;
		bool bMouseUp = false;
		switch (a_Msg)
		{
		case WM_LBUTTONDBLCLK:
			bDoubleClick = true;
			Button = EMouseButton::Left;
			break;
		case WM_LBUTTONDOWN:
			Button = EMouseButton::Left;
			break;
		case WM_LBUTTONUP:
			bMouseUp = true;
			Button = EMouseButton::Left;
			break;
		case WM_MBUTTONDBLCLK:
			bDoubleClick = true;
			Button = EMouseButton::Middle;
			break;
		case WM_MBUTTONDOWN:
			Button = EMouseButton::Middle;
			break;
		case WM_MBUTTONUP:
			bMouseUp = true;
			Button = EMouseButton::Middle;
			break;
		case WM_RBUTTONDBLCLK:
			bDoubleClick = true;
			Button = EMouseButton::Right;
			break;
		case WM_RBUTTONDOWN:
			Button = EMouseButton::Right;
			break;
		case WM_RBUTTONUP:
			bMouseUp = true;
			Button = EMouseButton::Right;
			break;
		case WM_XBUTTONDBLCLK:
			bDoubleClick = true;
			Button = (HIWORD(a_wParam) & XBUTTON1) ? EMouseButton::Mouse4 : EMouseButton::Mouse5;
			break;
		case WM_XBUTTONDOWN:
			Button = (HIWORD(a_wParam) & XBUTTON1) ? EMouseButton::Mouse4 : EMouseButton::Mouse5;
			break;
		case WM_XBUTTONUP:
			bMouseUp = true;
			Button = (HIWORD(a_wParam) & XBUTTON1) ? EMouseButton::Mouse4 : EMouseButton::Mouse5;
			break;
		}		
		
		const Math::Vec2 CursorPos(CursorPoint.x, CursorPoint.y);
		if (bMouseUp)
		{
			m_MessageHandler->OnMouseUp(Button, CursorPos);
		}
		else if (bDoubleClick)
		{
			m_MessageHandler->OnMouseDoubleClick(Window, Button, CursorPos);
		}
		else
		{
			m_MessageHandler->OnMouseDown(Window, Button, CursorPos);
		}
		
		return 0;
	}
	case WM_MOUSEMOVE:
		if (m_MessageHandler)
		{
			const i32 X = GET_X_LPARAM(a_lParam);
			const i32 Y = GET_Y_LPARAM(a_lParam);
			const i32 DX = s_PreviousX == -1 ? 0 : X - s_PreviousX;
			const i32 DY = s_PreviousY == -1 ? 0 : Y - s_PreviousY;
			m_MessageHandler->OnMouseMove(X, Y, DX, DX);
			s_PreviousX = X;
			s_PreviousY = Y;

			return 0;
		}
		break;
	case WM_WINDOWPOSCHANGED:
		if (((WINDOWPOS*)a_lParam)->hwnd == Window->GetOSHandle())
		{
			Window->SetPosition({ ((WINDOWPOS*)a_lParam)->x, ((WINDOWPOS*)a_lParam)->y});
		}		
		break;
	case WM_SIZE:
		break;
	case WM_ERASEBKGND:
		return 1;
	}

	return (u32)DefWindowProc(a_WindowHandle, a_Msg, a_wParam, a_lParam);
}

TSharedPtr<CWindowsWindow> CWindowsContext::GetWindowFromHwnd(HWND a_WindowHandle)
{
	for (auto& Wnd : m_PlatformWindows)
	{
		CWindowsWindow* WindowsWnd = static_cast<CWindowsWindow*>(Wnd.get());
		if (WindowsWnd != nullptr && WindowsWnd->GetWindowHandle() == a_WindowHandle)
		{
			return std::dynamic_pointer_cast<CWindowsWindow>(Wnd);
		}
	}

	return nullptr;
}

CWindowsContext::CWindowsContext(const HINSTANCE a_InstanceHandle, const HICON a_IconHandle) :
	m_InstanceHandle(a_InstanceHandle)
{
	// Register the window class
	RegisterWindowClass(m_InstanceHandle, a_IconHandle);

	// Initialize ole so windows support drag and drop
	HRESULT res = OleInitialize(NULL);
}

TSharedPtr<CWindow> CWindowsContext::GetWindowObject()
{
	usize Index = m_PlatformWindows.Num();
	m_PlatformWindows.Add(PlatformCore::CWindow::Get());
	return m_PlatformWindows[Index];
}

} }