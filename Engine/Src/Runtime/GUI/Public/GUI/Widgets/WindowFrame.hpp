#pragma once

#include <GUI/Widgets/Widget.hpp>

#include <Core/Definitions.hpp>

namespace Fade { inline namespace GUI {

/**
 * GUI Window Frame class
 * 
 * Handles window input such as resizing/minimizing/maximizing/closing
 */
class FADE_GUI_API CWindowFrame : public CWidget
{
public:
	CWindowFrame();
	//explicit CWindowFrame() noexcept;
	virtual ~CWindowFrame() override = default;

	virtual void Render(IRenderer* a_Renderer) override;
	virtual void Cleanup() override;

	// Input
	//virtual bool OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY) override;
	//virtual bool OnMouseButton(EMouseButton a_Button, bool a_Down, bool a_Repeat) override;
	//virtual bool OnKey(int a_Key, bool a_Down, bool a_Repeat) override;
	//virtual bool OnChar(wchar_t a_Char) override;
	//virtual bool OnMouseWheel(int a_Value) override;
	//virtual bool OnQuit() override;

private:
	bool m_IsHoveringExit : 1;
	bool m_IsHoveringMaximize : 1;
	bool m_IsHoveringMinimize : 1;
	bool m_IsHoveringRight : 1;
	bool m_IsHoveringLeft : 1;
	bool m_IsHoveringTop : 1;
	bool m_IsHoveringBot : 1;
};

} }