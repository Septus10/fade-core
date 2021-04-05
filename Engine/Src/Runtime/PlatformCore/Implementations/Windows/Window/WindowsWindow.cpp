#include "WindowsWindow.hpp"

// STL includes
#include <iostream>
#include <unordered_map>

// Engine includes
#include <PlatformCore/PlatformCore.hpp>
#include <Core/ServiceLocator/ServiceLocator.hpp>

using namespace Fade;
using namespace PlatformCore;

const TCHAR CWindowsWindow::sm_AppWindowClass[] = TEXT("FadeWindow");

CWindowsWindow::CWindowsWindow() :
	m_WindowHandle(nullptr)
{ }

CWindowsWindow::~CWindowsWindow()
{
	if (m_WindowHandle)
	{
		CWindowsWindow::Destroy();
	}
}

// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/ApplicationCore/Private/Windows/WindowsWindow.cpp
// line 35 & 36
static i32 WindowsAeroBorderSize		= 8;
static i32 WindowsStandardBorderSize	= 4;

bool CWindowsWindow::Create(SWindowSettings& a_Settings, CWindow* a_Parent)
{
	CWindowsWindow* ParentWindow = dynamic_cast<CWindowsWindow*>(a_Parent);
	m_WindowSettings = a_Settings;
	// If there's already a window created
	if (m_WindowHandle)
	{
		// Destroy it
		Destroy();
	}

	u32 WindowExStyle = 0;
	u32 WindowStyle = 0;

	u32 WidthToCreate = m_WindowSettings.m_Width;
	u32 HeightToCreate = m_WindowSettings.m_Height;
	u32 PosX = m_WindowSettings.m_PosX;
	u32 PosY = m_WindowSettings.m_PosY;

	if (!m_WindowSettings.m_HasBorder)
	{
		WindowExStyle = WS_EX_WINDOWEDGE;
		WindowStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
		if (m_WindowSettings.m_InTaskbar)
		{
			WindowExStyle |= WS_EX_APPWINDOW;
		}
		else
		{
			WindowExStyle |= WS_EX_TOOLWINDOW;
		}

		if (m_WindowSettings.m_IsTopmostWindow)
		{
			WindowExStyle |= WS_EX_TOPMOST;
		}

		if (m_WindowSettings.m_AcceptsInput)
		{
			WindowExStyle |= WS_EX_TRANSPARENT;
		}
	}
	else
	{
		WindowExStyle = WS_EX_APPWINDOW;
		WindowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;

		if (m_WindowSettings.m_IsRegular)
		{
			if (m_WindowSettings.m_SupportsMaximize)
			{
				WindowStyle |= WS_MAXIMIZEBOX;
			}

			if (m_WindowSettings.m_SupportsMinimize)
			{
				WindowStyle |= WS_MINIMIZEBOX;
			}

			if (m_WindowSettings.m_HasSizingFrame)
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

		m_WindowSettings.m_PosX -= BorderRect.left;
		m_WindowSettings.m_PosY -= BorderRect.top;
		m_WindowSettings.m_Width -= BorderRect.right - BorderRect.left;
		m_WindowSettings.m_Height -= BorderRect.bottom - BorderRect.top;
	}

	m_WindowHandle = CreateWindowEx(
		WindowExStyle,
		sm_AppWindowClass,
		m_WindowSettings.m_Title,
		WindowStyle,
		PosX, PosY,
		WidthToCreate, HeightToCreate,
		ParentWindow ? ParentWindow->GetWindowHandle() : nullptr,
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

	m_WindowPos.x = m_WindowSettings.m_PosX;
	m_WindowPos.y = m_WindowSettings.m_PosY;
	m_WindowSize.x = m_WindowSettings.m_Width;
	m_WindowSize.y = m_WindowSettings.m_Height;
	
	return true;
}

bool CWindowsWindow::Destroy()
{
	bool bDestroyed = ::DestroyWindow(m_WindowHandle);
	if (bDestroyed)
	{
		m_WindowHandle = nullptr;
	}
	
	return bDestroyed;
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

void CWindowsWindow::SetCapture(bool a_NewCapture)
{
	if (a_NewCapture)
	{
		::SetCapture(m_WindowHandle);
	}
	else
	{
		::ReleaseCapture();
	}
}

void CWindowsWindow::SetFocus()
{
	::SetFocus(m_WindowHandle);	
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

bool CWindowsWindow::IsCapture() const
{
	return ::GetCapture() == m_WindowHandle;
}

bool CWindowsWindow::IsFocus() const
{
	return ::GetFocus() == m_WindowHandle;
}

TSharedPtr<CWindow> CWindow::Get()
{
	return Fade::MakeShared<CWindowsWindow>();
}