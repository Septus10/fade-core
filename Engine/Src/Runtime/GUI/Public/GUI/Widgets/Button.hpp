#pragma once

#include <GUI/Widgets/Widget.hpp>

namespace Fade { namespace GUI {

struct STextureReference
{
	TSharedPtr<class CTexture> m_TextureReference;
	float m_U1, m_V1;
	float m_U2, m_V2;
	SColor m_Color;
};

class FADE_GUI_API CButton : public CWidget
{
public:
	CButton();

	virtual ~CButton() override = default;

	void Render(IRenderer* a_Renderer) override;
	void Cleanup() override;

	bool OnMouseMove(int a_X, int a_Y, int a_DX, int a_DY) override;
	bool OnMouseButtonDown(EMouseButton a_Button, bool a_Repeat) override;
	bool OnMouseButtonUp(EMouseButton a_Button) override;

private:
	STextureReference m_Texture;
	STextureReference m_OnHoverTexture;
	STextureReference m_OnClickTexture;
};

} }
