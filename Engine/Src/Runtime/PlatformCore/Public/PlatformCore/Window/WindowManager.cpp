#include "WindowManager.hpp"

#include <Windef.h>

namespace Fade { namespace PlatformCore {

CWindow::CWindow()
{ }

CWindow::~CWindow()
{ }

CWindowManager::~CWindowManager()
{
	DestroyAllWindows();
}

CWindow CWindowManager::MakeWindow(SWindowSettings & a_WindowSettings, CWindow * a_Parent)
{
	m_Windows.push_back(GetWindow());	
	m_Windows.back()->Create(a_WindowSettings, a_Parent);
	return m_Windows.back();
}

i32 CWindowManager::DestroyWindow(HWindow a_Idx)
{
	m_Windows[a_Idx]->Destroy();
	return 1;
}

CWindow* CWindowManager::GetWindowPtr(i32 a_Idx) const
{
	if (m_Windows.size() > a_Idx && a_Idx > 0)
	{
		return m_Windows[a_Idx];
	}

	// TODO
	// Log an error, window idx out of range
	return nullptr;
}


void CWindowManager::DestroyAllWindows()
{
	for (usize i = 0; i < m_Windows.size(); ++i)
	{
		DestroyWindow(i);
	}

	m_Windows.clear();
}

void CWindowManager::OnWindowDestroyed(void* a_WindowHandle)
{
	for (auto it = m_Windows.begin(); it != m_Windows.end(); ++it)
	{
		if ((*it)->GetWindowHandle() == a_WindowHandle)
		{
			m_Windows.erase(it);
			return;
		}
	}
}

} }