#include <editor.hpp>

#include <core/bootstrapping/bootstrapper.hpp>
#include <Core/fstl/Memory.hpp>

#include <iostream>
#include <sstream>

namespace Fade {

i32 CurPosX = 0;
float timer = 0;

ETickResult CEditor::Tick(double delta_time)
{
    return ETickResult::CONTINUE;
}

void CEditor::FixedTick(double fixed_delta_time)
{

}

void CEditor::PreInitialize()
{

}

void CEditor::Initialize()
{
	cur_time_ = 0;

	// Create main editor window
	{
		Fade::PlatformCore::SWindowSettings WindowSettings{};
		WindowSettings.m_HasBorder = true;
		WindowSettings.m_IsRegular = true;
		WindowSettings.m_SupportsMaximize = true;
		WindowSettings.m_SupportsMinimize = true;
		WindowSettings.m_HasSizingFrame = true;
		WindowSettings.m_PosX = 0;
		WindowSettings.m_PosY = 0;
		WindowSettings.m_Width = 1080;
		WindowSettings.m_Height = 720;
		m_WindowManager.MakeWindow(WindowSettings);
	}

	{
		Fade::PlatformCore::SWindowSettings WindowSettings{};
		WindowSettings.m_HasBorder = true;
		WindowSettings.m_IsRegular = true;
		WindowSettings.m_SupportsMaximize = true;
		WindowSettings.m_SupportsMinimize = true;
		WindowSettings.m_PosX = 0;
		WindowSettings.m_PosY = 0;
		WindowSettings.m_Width = 420;
		WindowSettings.m_Height = 240;
		m_WindowManager.MakeWindow(WindowSettings);
	}
}

void CEditor::PostInitialize()
{

}

void CEditor::DeInitialize()
{

}

}

/** Needs to be defined out of the namespace */
Fade::UniquePtr<Fade::CApplication> Fade::GetApplication()
{
	return Fade::MakeUnique<CEditor>();
}

