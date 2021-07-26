#include <GUI\Widgets\Button.hpp>

namespace Fade { namespace GUI {

CButton::CButton()
{
}

void CButton::Render(IRenderer* a_Renderer)
{
}

void CButton::Cleanup()
{
}

bool CButton::OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY)
{
	return false;
}

bool CButton::OnMouseButtonDown(EMouseButton a_Button, bool a_Repeat)
{
	return false;
}

bool CButton::OnMouseButtonUp(EMouseButton a_Button)
{
	return false;
}

} }

