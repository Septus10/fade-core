#include "WindowsWindow.hpp"

#include <PlatformCore/PlatformCore.hpp>

using namespace Fade;
using namespace PlatformCore;

CWindowsWindow::CWindowsWindow() :
	m_WindowHandle(nullptr)
{ }

CWindowsWindow::~CWindowsWindow()
{ }

const TCHAR CWindowsWindow::sm_AppWindowClass[] = TEXT("FadeWindow");

// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/ApplicationCore/Private/Windows/WindowsWindow.cpp
// line 35 & 36
static i32 WindowsAeroBorderSize		= 8;
static i32 WindowsStandardBorderSize	= 4;

bool CWindowsWindow::Create(SWindowSettings& a_Settings, CWindow* a_Parent)
{
	u32 WindowExStyle = 0;
	u32 WindowStyle = 0;

	if (!a_Settings.m_HasBorder)
	{
		WindowExStyle = WS_EX_WINDOWEDGE;
		WindowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		if (a_Settings.m_InTaskbar)
		{
			WindowExStyle |= WS_EX_APPWINDOW;
		}
		else
		{
			WindowExStyle |= WS_EX_TOOLWINDOW;
		}

		if (a_Settings.m_IsTopmostWindow)
		{
			WindowExStyle |= WS_EX_TOPMOST;
		}

		if (a_Settings.m_AcceptsInput)
		{
			WindowExStyle |= WS_EX_TRANSPARENT;
		}
	}
	else
	{
		WindowExStyle = WS_EX_APPWINDOW;
		WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;

		if (a_Settings.m_IsRegular)
		{
			if (a_Settings.m_SupportsMaximize)
			{
				WindowStyle |= WS_MAXIMIZEBOX;
			}

			if (a_Settings.m_SupportsMinimize)
			{
				WindowStyle |= WS_MINIMIZEBOX;
			}

			if (a_Settings.m_HasSizingFrame)
			{
				WindowStyle |= WS_THICKFRAME;
			}
			else
			{
				WindowStyle |= WS_BORDER;
			}
		}
		else
		{
			WindowStyle |= WS_POPUP | WS_BORDER;
		}

		RECT BorderRect = { 0 };
		::AdjustWindowRectEx(&BorderRect, WindowStyle, false, WindowExStyle);

		a_Settings.m_PosX -= BorderRect.left;
		a_Settings.m_PosY -= BorderRect.top;
		a_Settings.m_Width -= BorderRect.right - BorderRect.left;
		a_Settings.m_Width -= BorderRect.bottom - BorderRect.top;
	}

	m_WindowHandle = CreateWindowEx(
		WindowExStyle,
		sg_WindowClassName,
		a_Settings.m_Title,
		WindowStyle,
		a_Settings.m_PosX, a_Settings.m_PosY,
		a_Settings.m_Width, a_Settings.m_Height,
		a_Parent ? static_cast<HWND>(a_Parent->GetWindowHandle()) : nullptr,
		nullptr, GetModuleHandle(NULL), nullptr);

	if (m_WindowHandle == nullptr)
	{
		DWORD ErrorID = ::GetLastError();
		if (ErrorID == 0)
		{
			MessageBox(nullptr, TEXT("Unknown cause."), TEXT("Window Creation Error"), MB_OK | MB_ICONEXCLAMATION);
		}
		else
		{
			MessageBox(nullptr, GetLastErrorMessage(), TEXT("Window Creation Error"), MB_OK | MB_ICONEXCLAMATION);
		}

		return false;
	}

	return true;
}

bool CWindowsWindow::Destroy()
{
	::DestroyWindow(m_WindowHandle);
	return true;
}

void CWindowsWindow::Show()
{
	::ShowWindow(m_WindowHandle, SW_SHOW);
}

void CWindowsWindow::Hide()
{
	::ShowWindow(m_WindowHandle, SW_HIDE);
}

void CWindowsWindow::BringToFront()
{
}

void CWindowsWindow::Focus()
{
}

void CWindowsWindow::Reshape(i32 a_X, i32 a_Y, i32 a_Width, i32 a_Height)
{
}

void CWindowsWindow::Move(i32 a_X, i32 a_Y)
{
}

void CWindowsWindow::Minimize()
{
}

void CWindowsWindow::Maximize()
{
	::ShowWindow(m_WindowHandle, SW_MAXIMIZE);
}

HRESULT Fade::PlatformCore::CWindowsWindow::QueryInterface(REFIID iid, void ** ppvObject)
{
	return E_NOTIMPL;
}

ULONG Fade::PlatformCore::CWindowsWindow::AddRef(void)
{
	return 0;
}

ULONG Fade::PlatformCore::CWindowsWindow::Release(void)
{
	return 0;
}

HRESULT Fade::PlatformCore::CWindowsWindow::DragEnter(IDataObject * pDataObj, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	return E_NOTIMPL;
}

HRESULT Fade::PlatformCore::CWindowsWindow::DragLeave()
{
	return E_NOTIMPL;
}

HRESULT Fade::PlatformCore::CWindowsWindow::DragOver(DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	return E_NOTIMPL;
}

HRESULT Fade::PlatformCore::CWindowsWindow::Drop(IDataObject * pDataObj, DWORD grfKeyState, POINTL pt, DWORD * pdwEffect)
{
	return E_NOTIMPL;
}

Fade::PlatformCore::CWindow* GetWindow()
{
	return new CWindowsWindow();
}