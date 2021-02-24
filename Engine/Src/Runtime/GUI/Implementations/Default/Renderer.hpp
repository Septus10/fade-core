#pragma once

#include <GUI/IRenderer.hpp>

#include <RHI/IVertexBuffer.hpp>
#include <RHI/IIndexBuffer.hpp.>
#include <RHI/IFrameBuffer.hpp>
#include <RHI/Pipeline/ShaderProgram.hpp>

namespace Fade { inline namespace GUI {

class CRenderer : public IRenderer
{
public:
	CRenderer();

	virtual void Init() override;
	virtual void Begin(Fade::PlatformCore::CWindow* a_Window) override;
	virtual void End() override;

	// Drawing
	virtual void DrawFilledQuad(const Fade::SRect<u32>& a_Rect, const SColor& a_Color) override;
	//virtual void DrawTexturedQuad(const Graphics::CTexture& a_Texture, const GUI::SRect& a_Rect, const Math::Vec2& a_UV1, const Math::Vec2& a_UV2) override;
	virtual void DrawLinedQuad(const SRect<u32>& a_Rect) override;
	virtual void DrawPixel(Math::Vec2 a_Location) override;

private:
	void Flush();
	void AddVert(const float a_X, const float a_Y, 
		const float a_U, const float a_V, 
		const SColor& a_Color,
		const i8 a_TexID);

	u32 m_VertNum;
	usize m_MaxNumTextures;

	RHI::CShaderProgram m_Program;

	RHI::IFrameBuffer* m_FrameBuffer;
	TArray<RHI::SVertex> m_Vertices;
	TArray<usize> m_Indices;

	std::shared_ptr<IVertexBuffer> m_VertexBuffer;
	std::shared_ptr<IIndexBuffer> m_IndexBuffer;

	static const u32 m_MaxVertices;
};

} }