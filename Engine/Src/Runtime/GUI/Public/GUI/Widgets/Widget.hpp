#pragma once

#include <GUI/GUIApi.hpp>
#include <GUI/IRenderer.hpp>

#include <PlatformCore/Input/InputTypes.hpp>

namespace Fade { inline namespace GUI {

class FADE_GUI_API CWidget
{
public:
	virtual ~CWidget() = default;

	virtual void Render(IRenderer* a_Renderer) = 0;
	virtual void Cleanup() = 0;

	virtual const SRect<u32>& GetRect() const { return m_Rect; }
	virtual SRect<u32>& GetRect() { return m_Rect; }
	virtual void SetRect(const SRect<u32>& a_Rect) { m_Rect = a_Rect; }

	virtual bool OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY) { return false; }
	virtual bool OnMouseButtonDown(EMouseButton a_Button, bool a_Repeat) { return false; }
	virtual bool OnMouseButtonUp(EMouseButton a_Button) { return false; }
	virtual bool OnKeyDown(AKey a_Key, bool a_Repeat) { return false; }
	virtual bool OnKeyDoubleClick(AKey a_Key) { return false; }
	virtual bool OnKeyUp(AKey a_Key) { return false; }
	virtual bool OnChar(wchar_t a_Char, bool a_Repeat) { return false; }
	virtual bool OnMouseWheel(int a_Value) { return false; }
	virtual bool OnQuit() { return false; }

protected:
	/**
	 * Rect for our widget
	 */
	SRect<u32> m_Rect;
	
	struct SAnchor {
		float m_X;
		float m_Y;
	} m_Anchor;

};


} }