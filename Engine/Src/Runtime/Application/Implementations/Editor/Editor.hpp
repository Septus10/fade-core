#pragma once

#include <Application/ApplicationApi.hpp>
#include <GUIApplication/GUIApplication.hpp>

#include <Core/Containers/UniquePointer.hpp>
#include <PlatformCore/Window/Window.hpp>
#include <vector>

namespace Fade
{

class CWindow;	
class CEditor : public GUI::CGUIApplication
{
	using Parent = GUI::CGUIApplication;
public:
	CEditor() = default;

	~CEditor() override = default;

	// IApplication {
	virtual ETickResult Tick(float a_DeltaTime) override;

	virtual ETickResult PostTick(float a_DeltaTime) override;

	virtual EInitializationResult Initialize() override;

	virtual EInitializationResult PostInitialize() override;

	virtual void Deinitialize() override;
	// } IApplication

private:
	AWindowHandle m_MainWindow;
};

}
