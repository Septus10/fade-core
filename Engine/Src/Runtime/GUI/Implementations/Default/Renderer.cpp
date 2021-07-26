#include <Renderer.hpp>

#include <RHI/RHI.hpp>

namespace Fade { inline namespace GUI {

const u32 CRenderer::m_MaxVertices = 2048;

CRenderer::CRenderer()
	: m_VertNum(0)
	, m_MaxNumTextures(0)
	, m_FrameBuffer(nullptr)
{ 
	m_Vertices.reserve(m_MaxVertices);
	m_Indices.reserve(usize(m_MaxVertices * 1.5));
}

void CRenderer::Init()
{
	m_MaxNumTextures	= RHI::g_MaxTextureCount;
	m_VertexBuffer		= RHI::CreateVertexBuffer();

	m_Program.LoadShaderProgram("./Shaders/UI/Default/");
}

void CRenderer::Begin(Fade::PlatformCore::CWindow* a_Window)
{
	// If we have a frame buffer, clear it
	//m_FrameBuffer->Bind();
	if (a_Window == nullptr)
	{
		return;
	}

	m_Program.Bind();
	m_Program.SetFloatValue("aWidth", float(a_Window->GetWidth()));
	m_Program.SetFloatValue("aHeight", float(a_Window->GetHeight()));
}

void CRenderer::End()
{
	Flush();
	m_Program.Unbind();
	//m_FrameBuffer->Unbind();
}

void CRenderer::DrawFilledQuad(const SRect<u32>& a_Rect, const SColor& a_Color)
{
	if ((m_VertNum + 6) > m_MaxVertices)
	{
		Flush();
	}

	AddVert((float)a_Rect.m_Left, (float)a_Rect.m_Top, 0.f, 1.f, a_Color, 0);
	AddVert((float)a_Rect.m_Left, (float)a_Rect.m_Bottom, 0.f, 0.f, a_Color, 0);
	AddVert((float)a_Rect.m_Right, (float)a_Rect.m_Bottom, 1.f, 0.f, a_Color, 0);
	AddVert((float)a_Rect.m_Right, (float)a_Rect.m_Bottom, 1.f, 0.f, a_Color, 0);
	AddVert((float)a_Rect.m_Right, (float)a_Rect.m_Top, 1.f, 1.f, a_Color, 0);
	AddVert((float)a_Rect.m_Left, (float)a_Rect.m_Top, 0.f, 1.f, a_Color, 0);
}

//void CRenderer::DrawTexturedQuad(const Graphics::CTexture& a_Texture, const GUI::SRect & a_Rect, const Math::Vec2 & a_UV1, const Math::Vec2 & a_UV2)
//{
//
//}

void CRenderer::DrawLinedQuad(const SRect<u32>& a_Rect)
{
}

void CRenderer::DrawPixel(Math::Vec2 a_Location)
{
}

void CRenderer::Flush()
{
	m_VertexBuffer->SetData(m_Vertices.data(), m_Vertices.size());
	m_VertexBuffer->Draw();
}

void CRenderer::AddVert(const float a_X, const float a_Y,
	const float a_U, const float a_V,
	const SColor& a_Color,
	const i8 a_TexID)
{
#if FADE_DEBUG
	if (m_VertNum >= m_MaxVertices)
	{
		return;
	}
#endif

	u8 R = u8(a_Color.m_R * 255.f);
	u8 G = u8(a_Color.m_G * 255.f);
	u8 B = u8(a_Color.m_B * 255.f);
	u8 A = u8(a_Color.m_A * 255.f);

	u32 RGBA = R | G << 8 | B << 16 | A << 24;

	SVertex Vert 
	{ 
		a_X, a_Y, 0.5f,		// X Y Z
		a_U, a_V,			// U V
		RGBA,
		a_TexID				// TexID
	};

	m_Vertices.push_back(Vert);
}

TUniquePtr<IRenderer> CreateRenderer()
{
	return Fade::MakeUnique<CRenderer>();
}

} }