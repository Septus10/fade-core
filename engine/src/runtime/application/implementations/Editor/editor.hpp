#pragma once

#include <Application/Application.hpp>
#include <Application/ApplicationApi.hpp>

#include <PlatformCore/Window/WindowManager.hpp>

#include <Core/fstl/Memory.hpp>
#include <vector>

namespace Fade
{

class CWindow;	
class CEditor : public CApplication
{
public:
	CEditor() = default;

	~CEditor() = default;

	ETickResult Tick(double a_DeltaTime) override;

	void FixedTick(double a_FixedDeltaTime) override;

	void PreInitialize() override;

	void Initialize() override;

	void PostInitialize() override;

	void DeInitialize() override;

private:
	float cur_time_;
	bool* should_stop_;

	Fade::PlatformCore::CWindowManager m_WindowManager;
};

}
