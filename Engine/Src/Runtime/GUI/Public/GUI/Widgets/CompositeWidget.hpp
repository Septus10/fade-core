#pragma once

#include <GUI/Widgets/Widget.hpp>

namespace Fade { inline namespace GUI { 

class FADE_GUI_API CCompositeWidget : public CWidget
{
public:
	virtual ~CCompositeWidget();

	virtual void Render(IRenderer* a_Renderer);
	virtual void Cleanup();

	virtual bool OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY);
	virtual bool OnMouseButtonDown(EMouseButton a_Button, bool a_Repeat);
	virtual bool OnMouseButtonUp(EMouseButton a_Button);
	virtual bool OnKeyDown(AKey a_Key, bool a_Repeat);
	virtual bool OnKeyDoubleClick(AKey a_Key);
	virtual bool OnKeyUp(AKey a_Key);
	virtual bool OnChar(wchar_t a_Char, bool a_Repeat);
	virtual bool OnMouseWheel(int a_Value);
	virtual bool OnQuit();

private:
	TArray<TUniquePtr<CWidget>> m_Widgets;
};

} }