#include <GUI/Widgets/CompositeWidget.hpp>

namespace Fade { inline namespace GUI {

CCompositeWidget::~CCompositeWidget()
{
}

void CCompositeWidget::Render(IRenderer* a_Renderer)
{
	for (auto& Widget : m_Widgets)
	{
		Widget->Render(a_Renderer);
	}
}

void CCompositeWidget::Cleanup()
{
	for (auto& Widget : m_Widgets)
	{
		Widget->Cleanup();
	}

	m_Widgets.empty();
}

bool CCompositeWidget::OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY)
{
	return false;
}
bool CCompositeWidget::OnMouseButtonDown(EMouseButton a_Button, bool a_Repeat)
{
	return false;
}
bool CCompositeWidget::OnMouseButtonUp(EMouseButton a_Button)
{
	return false;
}
bool CCompositeWidget::OnKeyDown(AKey a_Key, bool a_Repeat)
{
	return false;
}
bool CCompositeWidget::OnKeyDoubleClick(AKey a_Key)
{
	return false;
}
bool CCompositeWidget::OnKeyUp(AKey a_Key)
{
	return false;
}
bool CCompositeWidget::OnChar(wchar_t a_Char, bool a_Repeat)
{
	return false;
}
bool CCompositeWidget::OnMouseWheel(int a_Value)
{
	return false;
}

bool CCompositeWidget::OnQuit()
{
	return false;
}

} }