#pragma once

// Platform core defines
#include <PlatformCore/PlatformCoreApi.hpp>
#include <PlatformCore/Window/WindowSettings.hpp>

// Core defines
#include <Core/Definitions.hpp>
#include <Core/Math/Math.hpp>
#include <Core/Containers/SharedPointer.hpp>
#include <Core/Containers/UniquePointer.hpp>

namespace Fade { inline namespace PlatformCore {

class FADE_PLATFORMCORE_API CWindow
{
public:
	static TSharedPtr<CWindow> Get();

	CWindow() = default;
	virtual ~CWindow() = default;

	virtual bool Create(SWindowSettings& a_Settings, CWindow* a_Parent = nullptr) = 0;

	virtual bool Destroy() = 0;

	virtual void Show() = 0;

	virtual void Hide() = 0;

	virtual void BringToFront() = 0;

	virtual void SetCapture(bool a_NewCapture) = 0;

	virtual void SetFocus() = 0;

	virtual void Reshape(i32 a_X, i32 a_Y, i32 a_Width, i32 a_Height) = 0;

	virtual void Move(i32 a_X, i32 a_Y) = 0;

	virtual void Minimize() = 0;

	virtual void Maximize() = 0;

	// Setters
	virtual void SetWindowMode(EWindowMode a_NewMode) {}

	virtual void SetOpacity(const float a_Opacity) {}

	virtual void SetEnabled(bool a_bIsEnabled) {}

	virtual void SetText(const u8* const a_Text) {}

	virtual void SetPosition(const Fade::Math::Vec2& a_Position) { m_WindowPos = a_Position; }

	// Getters
	virtual EWindowMode GetWindowMode() const { return EWindowMode::Fullscreen; }

	virtual bool IsWindowMaximized() const { return false; }

	virtual bool IsWindowMinimized() const { return false; }

	virtual bool IsVisible() const { return false; }

	virtual bool IsForegroundWindow() const { return false; }

	virtual bool IsCapture() const { return false; }

	virtual bool IsFocus() const { return false; }

	virtual void* GetOSHandle() const { return nullptr; }

	u32 GetWidth() const { return m_WindowSize.x; }
	u32 GetHeight() const { return m_WindowSize.y; }

	Fade::Math::iVec2 GetPosition() const { return m_WindowPos; }

protected:
	SWindowSettings					m_WindowSettings;

	Fade::Math::iVec2				m_WindowPos;
	Fade::Math::uVec2				m_WindowSize;
};



} }