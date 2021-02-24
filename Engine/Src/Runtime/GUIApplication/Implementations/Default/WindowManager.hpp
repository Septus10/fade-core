#pragma once

#include <GUI/Widgets/Widget.hpp>

#include <GUIApplication/GUIApplication.hpp>

#include <RHI/RHI.hpp>

#include <map>

namespace Fade { inline namespace GUI {

struct SWindowContainer
{
	// Default constructor
	SWindowContainer()
		: m_Window(nullptr)
		, m_BaseWidget(nullptr)
	{
	}

	// Copy constructor deleted since we're working with a unique pointer
	SWindowContainer(const SWindowContainer& a_Other) = delete;

	// Move constructor
	SWindowContainer(SWindowContainer&& a_Other) noexcept
	{
		m_WindowRenderContext = a_Other.m_WindowRenderContext;
		m_Window = Fade::Move(a_Other.m_Window);
		m_BaseWidget = Fade::Move(a_Other.m_BaseWidget);

		a_Other.m_WindowRenderContext = RHI::SWindowContext();
		a_Other.m_Window = nullptr;
		a_Other.m_BaseWidget = nullptr;
	}

	RHI::SWindowContext					m_WindowRenderContext;
	TSharedPtr<PlatformCore::CWindow>	m_Window;
	TUniquePtr<GUI::CWidget>			m_BaseWidget;
};

class CWindowManager
{
public:
	CWindowManager();

	AWindowHandle CreatePlatformWindow(TSharedPtr<PlatformCore::CWindow> a_Window, PlatformCore::SWindowSettings& a_WindowSettings, AWindowHandle a_ParentHandle);
	
	void RenderWindows(GUI::IRenderer* a_Renderer);
	
private:
	TArray<SWindowContainer> m_Windows;
	std::map<AWindowHandle, u32> m_WindowIndexMap;
};


} }
