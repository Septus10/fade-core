#include <WindowManager.hpp>

#include <Core/Logging/Log.hpp>

#include <GUI/Widgets/WindowFrame.hpp>

namespace Fade { inline namespace GUI {

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
	Container.m_BaseWidget = Fade::MakeUnique<CWindowFrame>();

	SRect<u32> Rect;
	Rect.m_Left = 0;
	Rect.m_Right = 1920;
	Rect.m_Bottom = 0;
	Rect.m_Top = 1040;

	Container.m_BaseWidget->SetRect(Rect);

	// Create the window using our settings
	bool bWindowCreated = Container.m_Window->Create(a_WindowSettings, Parent.get());
	if (bWindowCreated)
	{
		// Create RHI context for this window
		if (!Container.m_WindowRenderContext.Initialize(Container.m_Window.get()))
		{
			FADE_LOG(Error, GUI, "Unable to create window context.");
			Container.m_Window->Destroy();
			return false;
		}

		Container.m_Window->Show();
	}

	m_Windows.Add(Fade::Move(Container));
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

		a_Renderer->Begin(Ctx.m_Window.get());

		WindowContainer.m_BaseWidget->Render(a_Renderer);
		Ctx.CheckForErrors();

		a_Renderer->End();

		// Swap buffer
		Ctx.SwapBuffers();
		Ctx.CheckForErrors();
	}
}

} }