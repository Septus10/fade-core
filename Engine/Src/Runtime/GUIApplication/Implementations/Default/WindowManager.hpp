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

	~SWindowContainer()
	{
		if (m_Window.IsValid())
		{
			m_Window.Reset();
		}

		if (m_BaseWidget.IsValid())
		{
			m_BaseWidget.Reset();
		}
	}

	bool operator== (const SWindowContainer& a_Other)
	{
		return m_WindowRenderContext == a_Other.m_WindowRenderContext 
			&& m_Window == a_Other.m_Window
			&& m_BaseWidget == a_Other.m_BaseWidget;
	}

	RHI::SWindowContext					m_WindowRenderContext;
	TSharedPtr<PlatformCore::CWindow>	m_Window;
	TSharedPtr<GUI::CWidget>			m_BaseWidget;
};

class CWindowManager
{
public:
	CWindowManager();

	AWindowHandle CreatePlatformWindow(TSharedPtr<PlatformCore::CWindow> a_Window, PlatformCore::SWindowSettings& a_WindowSettings, AWindowHandle a_ParentHandle);
	
	AWindowHandle GetWindowHandleFromWindow(const TSharedPtr<PlatformCore::CWindow>& a_Window);

	bool CloseWindow(AWindowHandle a_Handle);
	bool CloseWindow(const TSharedPtr<PlatformCore::CWindow>& a_Window);

	void RenderWindows(GUI::IRenderer* a_Renderer);
	
	usize GetNumWindows() const;

private:
	TArray<SWindowContainer> m_Windows;
	std::map<AWindowHandle, u32> m_WindowIndexMap;
};


} }
