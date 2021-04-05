#include <WindowManager.hpp>

#include <Core/Logging/Log.hpp>

#include <PlatformCore/Application/PlatformContext.hpp>

#include <GUI/Widgets/WindowFrame.hpp>

namespace Fade { inline namespace GUI {

AWindowHandle HandleCounter = 100;

CWindowManager::CWindowManager()
{
	m_WindowIndexMap = std::map<AWindowHandle, u32>();
}

AWindowHandle Fade::GUI::CWindowManager::CreatePlatformWindow(TSharedPtr<PlatformCore::CWindow> a_Window, SWindowSettings& a_WindowSettings, AWindowHandle a_ParentHandle)
{
	// Try to get parent
	TSharedPtr<PlatformCore::CWindow> Parent;
	auto& it = m_WindowIndexMap.find(a_ParentHandle);
	if (it != m_WindowIndexMap.end())
	{
		u32 Idx = it->second;
		Parent = m_Windows[Idx].m_Window;
	}

	// Create new container
	SWindowContainer Container;
	Container.m_Window = a_Window;
	Container.m_BaseWidget = Fade::MakeShared<CWindowFrame>();
	
	SRect<u32> Rect;
	Rect.m_Left = 0;
	Rect.m_Right = 1920;
	Rect.m_Bottom = 0;
	Rect.m_Top = 1040;

	Container.m_BaseWidget->SetRect(Rect);

	// Create the window using our settings
	bool bWindowCreated = Container.m_Window->Create(a_WindowSettings, Parent.Get());
	if (bWindowCreated)
	{
		// Create RHI context for this window
		if (!Container.m_WindowRenderContext.Initialize(Container.m_Window.Get()))
		{
			FADE_LOG(Error, GUI, "Unable to create window context.");
			Container.m_Window->Destroy();
			return false;
		}

		Container.m_Window->Show();
	}

	// Temporary, if we ever make more windows than there are positive integers within 32 bits....
	// then we're screwed
	AWindowHandle NewHandle = ++HandleCounter;
	m_WindowIndexMap.insert(std::pair<AWindowHandle, u32>(HandleCounter, (u32)m_Windows.size()));
	m_Windows.push_back(Fade::Move(Container));

	return true;
}

AWindowHandle CWindowManager::GetWindowHandleFromWindow(const TSharedPtr<PlatformCore::CWindow>& a_Window)
{
	usize RequiredIndex = 0;
	for (usize i = 0; i < m_Windows.size(); ++i)
	{
		if (m_Windows[i].m_Window == a_Window)
		{
			RequiredIndex = i;
			break;
		}
	}

	for (auto& pair : m_WindowIndexMap)
	{
		if (pair.second == RequiredIndex)
		{
			return pair.first;
		}
	}

	return INVALID_HANDLE;
}

bool CWindowManager::CloseWindow(AWindowHandle a_Handle)
{
	auto& it = m_WindowIndexMap.find(a_Handle);
	if (it != m_WindowIndexMap.end())
	{
		u32 Index = it->second;
		SWindowContainer& Container = m_Windows[Index];
		if (Container.m_BaseWidget.IsValid())
		{
			Container.m_BaseWidget->Cleanup();
		}

		Container.m_WindowRenderContext.Destroy();
		if (Container.m_Window.IsValid())
		{
			Container.m_Window->Destroy();
		}

		m_Windows.erase(m_Windows.begin() + Index);

		return true;
	}

	return false;
}

bool CWindowManager::CloseWindow(const TSharedPtr<PlatformCore::CWindow>& a_Window)
{
	return CloseWindow(GetWindowHandleFromWindow(a_Window));
}

void CWindowManager::RenderWindows(GUI::IRenderer* a_Renderer)
{
	for (auto& WindowContainer : m_Windows)
	{
		RHI::SWindowContext Ctx = WindowContainer.m_WindowRenderContext;

		// Check if we even have a base widget to render
		if (!WindowContainer.m_BaseWidget.IsValid())
		{
			continue;
		}

		// Make current window's context current		
		Ctx.MakeCurrent();
		Ctx.CheckForErrors();

		a_Renderer->Begin(Ctx.m_Window.Get());

		WindowContainer.m_BaseWidget->Render(a_Renderer);
		Ctx.CheckForErrors();

		a_Renderer->End();

		// Swap buffer
		Ctx.SwapBuffers();
		Ctx.CheckForErrors();
	}
}

usize CWindowManager::GetNumWindows() const
{
	return m_Windows.size();
}

} }