#include <GUI/Widgets/WindowFrame.hpp>

#include <PlatformCore/PlatformCore.hpp>

namespace Fade { inline namespace GUI {

CWindowFrame::CWindowFrame()
	: m_IsHoveringExit(false)
	, m_IsHoveringMaximize(false)
	, m_IsHoveringMinimize(false)
	, m_IsHoveringRight(false)
	, m_IsHoveringLeft(false)
	, m_IsHoveringTop(false)
	, m_IsHoveringBot(false)
{ }

/*CWindowFrame::CWindowFrame() noexcept
	: m_IsHoveringExit(false)
	, m_IsHoveringMaximize(false)
	, m_IsHoveringMinimize(false)
	, m_IsHoveringRight(false)
	, m_IsHoveringLeft(false)
	, m_IsHoveringTop(false)
	, m_IsHoveringBot(false)
{ }*/

void CWindowFrame::Render(IRenderer* a_Renderer)
{
	// draw top left corner
	//a_Renderer->DrawFilledQuad(SRect(-1.0f, -0.99f, 1.0f, 0.99f), Fade::SColor::Cyan);

	// Draw logo top left

	// Draw top bar
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Left, m_Rect.m_Right, m_Rect.m_Top, m_Rect.m_Top - 4), Fade::SColor::Cyan);

	// Draw top right buttons
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Right - 64, m_Rect.m_Right - 4, m_Rect.m_Top - 4, m_Rect.m_Top - 24), SColor::Cyan);
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Right - 20, m_Rect.m_Right - 4, m_Rect.m_Top - 4, m_Rect.m_Top - 20), SColor::Black);
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Right - 19, m_Rect.m_Right - 5, m_Rect.m_Top - 5, m_Rect.m_Top - 19), m_IsHoveringExit ? SColor::Magenta : SColor::Red);
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Right - 40, m_Rect.m_Right - 24, m_Rect.m_Top - 4, m_Rect.m_Top - 20), SColor::Black);
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Right - 39, m_Rect.m_Right - 25, m_Rect.m_Top - 5, m_Rect.m_Top - 19), SColor::Yellow);
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Right - 60, m_Rect.m_Right - 44, m_Rect.m_Top - 4, m_Rect.m_Top - 20), SColor::Black);
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Right - 59, m_Rect.m_Right - 45, m_Rect.m_Top - 5, m_Rect.m_Top - 19), SColor::Green);

	// Draw top right corner

	// Draw right bar
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Right - 4, m_Rect.m_Right, m_Rect.m_Top, m_Rect.m_Bottom), Fade::SColor::Cyan);

	// Draw bottom right corner

	// Draw bottom bar
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Left, m_Rect.m_Right, m_Rect.m_Bottom + 4, m_Rect.m_Bottom), Fade::SColor::Cyan);

	// Draw bottom left corner

	// Draw left bar
	a_Renderer->DrawFilledQuad(SRect(m_Rect.m_Left, m_Rect.m_Left + 4, m_Rect.m_Top, m_Rect.m_Bottom), Fade::SColor::Cyan);
}

/*
bool CWindowFrame::OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY)
{
	a_Y = m_Rect.m_Top - a_Y;
	m_IsHoveringRight = a_X >= (int)m_Rect.m_Right - 4;
	m_IsHoveringLeft = a_X <= (int)m_Rect.m_Left + 4;
	m_IsHoveringTop = a_Y >= (int)m_Rect.m_Top - 4;
	m_IsHoveringBot = a_Y <= (int)m_Rect.m_Bottom + 4;
	
	m_IsHoveringExit = 
			a_X >= ((int)m_Rect.m_Right - 20) && 
			a_X <= ((int)m_Rect.m_Right - 4) &&
			a_Y >= ((int)m_Rect.m_Top - 20) && 
			a_Y <= ((int)m_Rect.m_Top - 4);

	if (m_IsHoveringRight && m_IsHoveringTop ||
		m_IsHoveringLeft && m_IsHoveringBot)
	{
		Fade::PlatformCore::SetCursorType(PlatformCore::ECursorType::ResizeNESW);
	}
	else if (m_IsHoveringRight && m_IsHoveringBot ||
		m_IsHoveringLeft && m_IsHoveringTop)
	{
		Fade::PlatformCore::SetCursorType(PlatformCore::ECursorType::ResizeNWSE);
	}
	else if (m_IsHoveringRight || m_IsHoveringLeft)
	{
		Fade::PlatformCore::SetCursorType(PlatformCore::ECursorType::ResizeLeftRight);
	}
	else if (m_IsHoveringTop || m_IsHoveringBot)
	{
		Fade::PlatformCore::SetCursorType(PlatformCore::ECursorType::ResizeUpDown);
	}
	else
	{
		Fade::PlatformCore::SetCursorType(PlatformCore::ECursorType::Pointer);
	}

	return false;
}

bool CWindowFrame::OnMouseButton(EMouseButton a_Button, bool a_Down, bool a_Repeat)
{
	if (a_Button == EMouseButton::Left && !a_Down)
	{
		if (m_IsHoveringExit)
		{
			// Close the window
			m_Window->Close();
		}
		else if (m_IsHoveringMaximize)
		{
			// Maximize the window

		}
		else if (m_IsHoveringMinimize)
		{
			// Minimize the window

		}
	}

	return false;
}

bool CWindowFrame::OnKey(int a_Key, bool a_Down, bool a_Repeat)
{
	return false;
}

bool CWindowFrame::OnChar(wchar_t a_Char)
{
	return false;
}

bool CWindowFrame::OnMouseWheel(int a_Value)
{
	return false;
}

bool CWindowFrame::OnQuit()
{
	return false;
}
*/

void CWindowFrame::Cleanup()
{
	
}

} }