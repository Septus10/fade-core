#include <application/application.hpp>

namespace Fade {

//u32 CApplication::CreateWindow(i32 a_ParentIndex)
//{
//	// Create main editor window
//	Fade::PlatformCore::SWindowSettings WindowSettings{};
//	auto NewWindow = Fade::MakeUnique<Fade::PlatformCore::CWindow>();
//	WindowSettings.m_HasBorder = true;
//	WindowSettings.m_IsRegular = true;
//	WindowSettings.m_SupportsMaximize = true;
//	WindowSettings.m_SupportsMinimize = true;
//	WindowSettings.m_HasSizingFrame = true;
//	WindowSettings.m_PosX = 0;
//	WindowSettings.m_PosY = 0;
//	WindowSettings.m_Width = 1080;
//	WindowSettings.m_Height = 720;
//	NewWindow->Create(WindowSettings, a_ParentIndex >= 0 ? m_Windows[a_ParentIndex].Get() : nullptr);
//	NewWindow->Show();
//	u32 NewWindowIndex = m_Windows.size();
//	m_Windows.Add(NewWindow);
//	return NewWindowIndex;
//}

}