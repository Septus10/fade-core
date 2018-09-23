#pragma once

#include <PlatformCore/PlatformCoreApi.hpp>
#include <Core/Core.hpp>

namespace Fade { namespace PlatformCore {

enum class EWindowMode
{
	Fullscreen,
	WindowedFullscreen,
	Windowed,
	NumWindowModes
};

enum class EWindowType
{
	Normal,
	Menu,
	Tooltip,
	Notification,
	Game,
	NumWindowTypes
};

struct FADE_PLATFORMCORE_API SWindowSettings
{
	EWindowType m_WindowType;

	// Window rect settings
	i32 m_PosX;
	i32 m_PosY;
	i32 m_Width;
	i32 m_Height;

	// Window options
	bool m_HasBorder;
	bool m_InTaskbar;
	bool m_IsTopmostWindow;
	bool m_AcceptsInput;
	bool m_FocusWhenShown;
	bool m_HasCloseButton;
	bool m_SupportsMinimize;
	bool m_SupportsMaximize;
	bool m_HasSizingFrame;

	bool m_IsModal;
	bool m_IsRegular;

	// Window generic settings
	const c8*	m_Title;
	f32			m_Opacity;
	bool		m_bIsEnabled;	
};

} }