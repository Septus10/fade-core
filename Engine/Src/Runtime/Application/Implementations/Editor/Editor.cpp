#include <Editor.hpp>

#include <Application/ApplicationFactory.hpp>

#include <Core/FileWatcher/FileWatcher.h>
#include <Core/Containers/UniquePointer.hpp>

#include <ECS/ECSUtil.hpp>
#include <ECS/System.hpp>
#include <ECS/Component.hpp>
#include <ECS/ComponentStore.hpp>

#include <GUIApplication/GUIApplication.hpp>

#include <PlatformCore/PlatformCore.hpp>
#include <PlatformCore/Window/Window.hpp>

#include <iostream>

namespace Fade {

ETickResult CEditor::Tick(float a_DeltaTime)
{
	ETickResult ParentResult = CGUIApplication::Tick(a_DeltaTime);

	FW::FileWatcher::Get().update();

	return ParentResult;
}

ETickResult CEditor::PostTick(float a_DeltaTime)
{
	CGUIApplication::PostTick(a_DeltaTime);

	return ETickResult::CONTINUE;
}

EInitializationResult CEditor::Initialize()
{
	Parent::Initialize();

	{
		Fade::PlatformCore::SWindowSettings WindowSettings{};
		WindowSettings.m_Title = "Fade Engine";
		WindowSettings.m_HasBorder = false;
		WindowSettings.m_IsRegular = true;
		WindowSettings.m_SupportsMaximize = true;
		WindowSettings.m_SupportsMinimize = true;
		WindowSettings.m_HasSizingFrame = false;
		WindowSettings.m_InTaskbar = true;
		//WindowSettings.m_IsTopmostWindow = true;
		WindowSettings.m_PosX = 0;
		WindowSettings.m_PosY = 0;
		WindowSettings.m_Width = 1920;
		WindowSettings.m_Height = 1040;

		m_MainWindow = CreatePlatformWindow(WindowSettings);
	}

	return EInitializationResult::SUCCESS;
}

EInitializationResult CEditor::PostInitialize()
{
	Parent::PostInitialize();

	return EInitializationResult::SUCCESS;
}

void CEditor::Deinitialize()
{
	Parent::Deinitialize();
}

}

/** Needs to be defined out of the namespace */
Fade::TUniquePtr<Fade::IApplication> Fade::GetApplication()
{
	return Fade::MakeUnique<Fade::CEditor>();
}

