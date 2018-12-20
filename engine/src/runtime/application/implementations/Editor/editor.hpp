#pragma once

#include <Application/Application.hpp>
#include <Application/ApplicationApi.hpp>

#include <Core/Containers/UniquePointer.hpp>
#include <PlatformCore/Window/Window.hpp>
#include <vector>

namespace Fade
{

class CWindow;	
class FADE_APPLICATION_API CEditor : public CApplication
{
public:
	CEditor() = default;

	~CEditor() = default;

	ETickResult Tick(double a_DeltaTime) override;

	void FixedTick(double a_FixedDeltaTime) override;

	bool PreInitialize() override;

	bool Initialize() override;

	bool PostInitialize() override;

	bool DeInitialize() override;

	const PlatformCore::CWindow* GetMainWindowPtr() const override { return m_MainWindow.Get(); }

private:
	float m_CurTime;
	bool* m_ShouldStop;

	Fade::TUniquePtr<Fade::PlatformCore::CWindow> m_MainWindow;
};

}
