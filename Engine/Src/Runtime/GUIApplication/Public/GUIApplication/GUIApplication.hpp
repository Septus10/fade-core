#pragma once

// GUIApplication includes
#include <GUIApplication/GUIApplicationApi.hpp>

// GUI includes
#include <GUI/Widgets/Widget.hpp>

// Application base includes
#include <ApplicationBase/Application.hpp>

// PlatformCore includes
#include <PlatformCore/Application/PlatformContext.hpp>
#include <PlatformCore/Application/PlatformMessageHandler.hpp>
#include <PlatformCore/Input/InputTypes.hpp>
#include <PlatformCore/Window/Window.hpp>

namespace Fade { 

using AWindowHandle = i32;

inline namespace GUI {

class FADE_GUIAPPLICATION_API CGUIApplication : public 
	ApplicationBase::IApplication,
	PlatformCore::IPlatformMessageHandler
{
	using Parent = IApplication;
public:
	CGUIApplication();

	/*
	 * Virtual destructor, required for inheritance
	 */
	virtual ~CGUIApplication() override;

	/**
	 * initialization function
	 */
	virtual EInitializationResult Initialize() override;

	/**
	 * post initialization function
	 */
	virtual EInitializationResult PostInitialize() override;

	/**
	 *
	 */
	virtual void Deinitialize() override;

	/**
	 *
	 */
	virtual ETickResult Tick(float a_DeltaTime) override;

	/**
	 *
	 */
	virtual ETickResult PostTick(float a_DeltaTime) override;

	/**
	 *
	 */
	void CloseWindow(AWindowHandle a_Window);

protected:
	/**
	 *
	 */
	AWindowHandle CreatePlatformWindow(PlatformCore::SWindowSettings& a_Settings, AWindowHandle a_Parent = -1);

	/** IPlatformMessageHandler interface */
	virtual bool OnKeyDown(const u32 a_Key) override;
	virtual bool OnKeyUp(const u32 a_Key) override;
	virtual bool OnMouseMove(const u32 a_NewX, const u32 a_NewY, const u32 a_DX, const u32 a_DY) override;
	virtual bool OnMouseDown(TSharedPtr<PlatformCore::CWindow> a_Window, EMouseButton a_Button, Fade::Math::iVec2 a_Location) override;
	virtual bool OnMouseUp(EMouseButton a_Button, Fade::Math::iVec2 a_Location) override;
	/**/

private:
	/**
	 *
	 */
	TUniquePtr<class CWindowManager> m_WindowManager;

	/** 
	 * 
	 */
	TUniquePtr<GUI::IRenderer> m_Renderer;
};

} } 