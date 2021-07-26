#include <GUI/Widgets/CompositeWidget.hpp>

namespace Fade { inline namespace GUI { 

CCompositeWidget::~CCompositeWidget()
{
	if (!m_Widgets.empty())
	{
		Cleanup();
	}
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

	m_Widgets.clear();
}

void CCompositeWidget::AddWidget(CWidget* a_Widget)
{
	m_Widgets.Add(MakeUnique<CWidget>(a_Widget));
}

bool CCompositeWidget::OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY)
{
	for (auto& Widget : m_Widgets)
	{
		if (Widget->OnMouseMove(a_X, a_Y, a_DX, a_DY))
		{
			return true;
		}
	}

	return false;
}

bool CCompositeWidget::OnMouseButtonDown(EMouseButton a_Button, bool a_Repeat)
{
	for (auto& Widget : m_Widgets)
	{
		if (Widget->OnMouseButtonDown(a_Button, a_Repeat))
		{
			return true;
		}
	}

	return false;
}

bool CCompositeWidget::OnMouseButtonUp(EMouseButton a_Button)
{
	for (auto& Widget : m_Widgets)
	{
		if (Widget->OnMouseButtonUp(a_Button))
		{
			return true;
		}
	}

	return false;
}

} }