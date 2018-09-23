#pragma once

#include <Core/Definitions.hpp>
#include <Core/fstl/Memory.hpp>
#include <PlatformCore/PlatformCoreApi.hpp>
#include <PlatformCore/Window/WindowSettings.hpp>

namespace Fade { namespace PlatformCore {

static const char* sg_WindowClassName = "FadeWindow";

class FADE_PLATFORMCORE_API CWindow
{
public:
	CWindow();
	virtual ~CWindow();

	virtual bool Create(SWindowSettings& a_Settings, CWindow* a_Parent = nullptr) = 0;

	virtual bool Destroy() = 0;

	virtual void Show() = 0;

	virtual void Hide() = 0;

	virtual void BringToFront() = 0;

	virtual void Focus() = 0;

	virtual void Reshape(i32 a_X, i32 a_Y, i32 a_Width, i32 a_Height) = 0;

	virtual void Move(i32 a_X, i32 a_Y) = 0;

	virtual void Minimize() = 0;

	virtual void Maximize() = 0;

	// Setters
	virtual void SetWindowMode(EWindowMode a_NewMode) {}

	virtual void SetOpacity(const float a_Opacity) {}

	virtual void SetEnabled(bool a_bIsEnabled) {}

	virtual void SetText(const u8* const a_Text) {}

	// Getters
	virtual EWindowMode GetWindowMode() const { return EWindowMode::Fullscreen; }

	virtual bool IsMaximized() const { return false; }

	virtual bool IsMinimized() const { return false; }

	virtual bool IsVisible() const { return false; }

	virtual bool IsForegroundWindow() const { return false; }

	virtual void* GetWindowHandle() const { return nullptr; }

protected:
	SWindowSettings m_WindowSettings;
};

} }

FADE_PLATFORMCORE_API Fade::PlatformCore::CWindow* GetWindow();