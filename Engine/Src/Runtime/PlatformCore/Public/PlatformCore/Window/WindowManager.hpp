#pragma once

#include <PlatformCore/PlatformCoreApi.hpp>
#include "Window.hpp"

#include <Core/fstl/Vector.hpp>
#include <Core/fstl/Memory.hpp> 

#include <Core/ServiceLocator/Service.hpp>

namespace Fade { namespace PlatformCore {

class FADE_PLATFORMCORE_API CWindowManager : public CService
{
public:
	~CWindowManager();

	CWindow* MakeWindow(SWindowSettings& a_WindowSettings, CWindow* a_Parent = nullptr);

	i32 DestroyWindow(CWindow* a_Window);

	CWindow* GetWindowPtr(i32 a_Idx) const;	

	void DestroyAllWindows();

	void OnWindowDestroyed(void* a_WindowHandle);

protected:
	fstl::Vector<CWindow*> m_Windows;
};

} } 