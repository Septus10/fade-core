#pragma once

#include <GUI/Widgets/Widget.hpp>

#include <Core/Containers/UniquePointer.hpp>

namespace Fade { inline namespace GUI { 

class FADE_GUI_API CCompositeWidget : public CWidget
{
public:
	CCompositeWidget()
	{ }

	CCompositeWidget(const CCompositeWidget& a_Other) = delete;
	CCompositeWidget& operator= (const CCompositeWidget& a_Other) = delete;
	~CCompositeWidget() override;

	void Render(IRenderer* a_Renderer) override;
	void Cleanup() override;

	void AddWidget(CWidget* a_Widget);

	bool OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY) override;
	bool OnMouseButtonDown(EMouseButton a_Button, bool a_Repeat) override;
	bool OnMouseButtonUp(EMouseButton a_Button) override;

private:
	TArray<TUniquePtr<CWidget>> m_Widgets;
};

} }