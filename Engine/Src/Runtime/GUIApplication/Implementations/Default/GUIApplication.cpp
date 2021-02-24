#include <Core/Log.hpp>

#include <GUIApplication/GUIApplication.hpp>

#include <GUI/IRenderer.hpp>

#include <PlatformCore/PlatformCore.hpp>
#include <PlatformCore/Window/Window.hpp>

#include <RHI/RHI.hpp>

#include <iostream>

#include <dinput.h>

#include <WindowManager.hpp>

namespace Fade { inline namespace GUI {

CGUIApplication::CGUIApplication()
{
}

CGUIApplication::~CGUIApplication()
{
}

EInitializationResult CGUIApplication::Initialize()
{
	sm_PlatformContext = Fade::MakeUnique(PlatformCore::CreatePlatformContext());
	sm_PlatformContext->SetMessageHandler(this);

	RHI::InitializeAPI();

	m_WindowManager = Fade::MakeUnique<GUI::CWindowManager>();
	m_Renderer = GUI::CreateRenderer();
	m_Renderer->Init();

	return EInitializationResult::SUCCESS;
}

EInitializationResult CGUIApplication::PostInitialize()
{
	return EInitializationResult::SUCCESS;
}

void CGUIApplication::Deinitialize()
{

}

ETickResult CGUIApplication::Tick(float a_DeltaTime)
{
	return sm_PlatformContext->IsQuitRequested() ? ETickResult::STOP : ETickResult::CONTINUE;
}

ETickResult CGUIApplication::PostTick(float a_DeltaTime)
{
	m_WindowManager->RenderWindows(m_Renderer.Get());
	return sm_PlatformContext->IsQuitRequested() ? ETickResult::STOP : ETickResult::CONTINUE;
}

AWindowHandle CGUIApplication::CreatePlatformWindow(PlatformCore::SWindowSettings& a_Settings, AWindowHandle a_Parent)
{
	TSharedPtr<CWindow> Window = sm_PlatformContext->GetWindowObject();
	return m_WindowManager->CreatePlatformWindow(Window, a_Settings, a_Parent);
}

void CGUIApplication::CloseWindow(AWindowHandle a_Window)
{
	// TODO 
	// Close window in window manager

}

bool CGUIApplication::OnKeyDown(const u32 a_Key)
{	
	return true;
}

bool CGUIApplication::OnKeyUp(const u32 a_Key)
{
	return false;
}

bool CGUIApplication::OnMouseMove(const u32 a_NewX, const u32 a_NewY, const u32 a_DX, const u32 a_DY)
{
		

	return true;
}

bool CGUIApplication::OnMouseDown(TSharedPtr<PlatformCore::CWindow> a_Window, EMouseButton a_Button, Fade::Math::iVec2 a_Location)
{
	TUniquePtr<PlatformCore::CPlatformContext>& PlatformContext = sm_PlatformContext;

	// If we have a valid window, set it as the current focus if it isn't already
	// this is how windows works as well
	if (IsValid(a_Window) && !a_Window->IsCapture())
	{
		a_Window->SetCapture(true);
	}
	
	return true;
}

bool CGUIApplication::OnMouseUp(EMouseButton a_Button, Fade::Math::iVec2 a_Location)
{
	//for (auto& UIWindow : m_UIWindows)
	//{
	//	// Check if it's within the window and if it is, whether or not it's in the top window
	//	PlatformCore::CWindow* Window = UIWindow->GetPlatformWindow();
	//	Fade::Math::iVec2 WindowPos = Window->GetPosition();
	//
	//	Fade::SRect<i32> WindowRect(WindowPos.x, Window->GetWidth(), Window->GetHeight(), WindowPos.y);
	//	if (WindowRect.Contains(a_Location))
	//	{
	//		UIWindow->OnMouseButton(a_Button, false, false);
	//	}
	//}
	return true;
}


} }