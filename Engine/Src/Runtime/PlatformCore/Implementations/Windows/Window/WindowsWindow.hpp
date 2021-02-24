#pragma once

#ifdef FADE_PLATFORM_WINDOWS

#include <PlatformCore/Window/Window.hpp>
#include <Windows.h> 

#include <Ole2.h>
#include <oleidl.h>

namespace Fade { inline namespace PlatformCore {

class FADE_PLATFORMCORE_API CWindowsWindow : public CWindow
{
public:
	CWindowsWindow();

	~CWindowsWindow() override;

	virtual bool Create(SWindowSettings& a_Settings, CWindow* a_Parent = nullptr) override;

	virtual bool Destroy() override;

	virtual void Show() override;

	virtual void Hide() override;

	virtual void BringToFront() override;

	virtual void SetCapture(bool a_NewCapture) override;

	virtual void SetFocus() override;

	virtual void Reshape(i32 a_X, i32 a_Y, i32 a_Width, i32 a_Height) override;

	virtual void Move(i32 a_X, i32 a_Y) override;

	virtual void Minimize() override;

	virtual void Maximize() override;

	virtual bool IsCapture() const override;

	virtual bool IsFocus() const override;

	virtual void* GetOSHandle() const { return m_WindowHandle; }

	HWND GetWindowHandle() const { return m_WindowHandle; }

private:
	HWND m_WindowHandle;

	static const TCHAR sm_AppWindowClass[];
};

} }

#endif