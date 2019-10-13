#pragma once

#include <Application/Application.hpp>
#include <Application/ApplicationApi.hpp>

#include <Core/Containers/UniquePointer.hpp>
#include <PlatformCore/Window/Window.hpp>
#include <vector>

#include <glm.hpp>

namespace Fade
{

class CWindow;	
class CEditor : public CApplication
{
public:
	CEditor() = default;

	~CEditor() override = default;

	ETickResult Tick(double a_DeltaTime) override;

	void FixedTick(double a_FixedDeltaTime) override;

	bool PreInitialize() override;

	bool Initialize() override;

	bool PostInitialize() override;

	bool DeInitialize() override;

	const PlatformCore::CWindow* GetMainWindowPtr() const override { return m_MainWindow.Get(); }

	void OnKeyUp(i32 a_KeyIdx) override;
	void OnKeyDown(i32 a_KeyIdx) override;

private:
	float m_CurTime;
	glm::vec2 m_CamPos;

	Fade::TUniquePtr<Fade::PlatformCore::CWindow> m_MainWindow;
};

}
