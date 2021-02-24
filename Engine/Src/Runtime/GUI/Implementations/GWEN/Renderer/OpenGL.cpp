#include <GUI/Renderers/OpenGL.h>
#include <GUI/Utility.h>
#include <GUI/Font.h>
#include <GUI/Texture.h>
#include <GUI/WindowProvider.h>

#include <math.h>
#include <iostream>

#include <FreeImage.h>

#include <Graphics/Types.hpp>
#include <Graphics/VertexBuffer.hpp>
#include <Graphics/GraphicsApiContext.hpp>
#include <Graphics/Pipeline/ShaderProgram.hpp>

using namespace Fade;

namespace Gwen { namespace Renderer {

Graphics::Pipeline::CShaderProgram g_ShaderProgram;

OpenGL::OpenGL(Graphics::CGraphicsAPIContext* a_ApiContext)
	: m_ApiContext(a_ApiContext)
{
	m_pContext = NULL;
	::FreeImage_Initialise();
}

OpenGL::~OpenGL()
{
	::FreeImage_DeInitialise();
}

void OpenGL::Init()
{
	m_Vertices.reserve(1024);
	g_ShaderProgram.LoadShaderProgram(".\\Shaders\\UI\\Default");
}

void OpenGL::Begin()
{
	m_ApiContext->EnableAlphaBlend();
	m_ApiContext->CheckForError("Alpha blend");
}

void OpenGL::End()
{
	Flush();
}

void OpenGL::Flush()
{
	if (m_Vertices.Num() == 0) { return; }

	m_ApiContext->CheckForError("Flush begin");

	std::shared_ptr<Graphics::CVertexBuffer> buffer = Graphics::CreateVertexBuffer(m_Vertices);

	m_ApiContext->CheckForError("Vertex buffer creation");

	g_ShaderProgram.Use();

	g_ShaderProgram.SetFloatValue("Width", 1225.f);
	g_ShaderProgram.SetFloatValue("Height", 960.f);

	m_ApiContext->CheckForError("Shader program use");

	Graphics::DrawVertexBuffer(buffer);

	m_ApiContext->CheckForError("Draw vertex buffer");

	m_Vertices.clear();

	m_ApiContext->CheckForError("Flush");
}

void OpenGL::AddVert(int x, int y, float u, float v)
{
	if (m_Vertices.Num() >= MaxVerts - 1)
	{
		Flush();
	}

	Fade::Graphics::SVertex vert;
	vert.X = (float)x;
	vert.Y = (float)y;
	vert.Z = 0.f;
	vert.U = u;
	vert.V = v;
	vert.R = m_Color.r;
	vert.G = m_Color.g;
	vert.B = m_Color.b;
	vert.A = m_Color.a;

	m_Vertices.Add(vert);
}

void OpenGL::DrawFilledRect(Gwen::Rect rect)
{
	if (m_ApiContext->AreTexturesEnabled())
	{
		Flush();
		m_ApiContext->DisableTextures();
		m_ApiContext->CheckForError("Disable textures");
	}

	Translate(rect);
	AddVert(rect.x, rect.y);
	AddVert(rect.x + rect.w, rect.y);
	AddVert(rect.x, rect.y + rect.h);
	AddVert(rect.x + rect.w, rect.y);
	AddVert(rect.x + rect.w, rect.y + rect.h);
	AddVert(rect.x, rect.y + rect.h);
}

void OpenGL::SetDrawColor(Gwen::Color color)
{
	m_ApiContext->SetDrawColorRGBA((unsigned char*)&color);
	m_Color = color;
}

void OpenGL::StartClip()
{
	Flush();
	Gwen::Rect rect = ClipRegion();
	// OpenGL's coords are from the bottom left
	// so we need to translate them here.
	{
		Graphics::SIRect viewport = m_ApiContext->GetViewportRect();
		rect.y = viewport.m_Width - (rect.y + rect.h);
	}
	m_ApiContext->ScissorTest(rect.x * Scale(), rect.y * Scale(), rect.w * Scale(), rect.h * Scale());
	m_ApiContext->EnableScissorTest();
};

void OpenGL::EndClip()
{
	Flush();
	m_ApiContext->DisableScissorTest();
};

void OpenGL::DrawTexturedRect(Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2)
{
	u32* tex = (u32*)pTexture->data;

	// Missing image, not loaded properly?
	if (!tex)
	{
		return DrawMissingImage(rect);
	}

	Translate(rect);
	
	bool texturesOn = m_ApiContext->AreTexturesEnabled();
	i32 boundtex = m_ApiContext->GetCurrentlyBoundTexture();

	if (!texturesOn || *tex != boundtex)
	{
		Flush();
		m_ApiContext->BindTexture(*tex);
		m_ApiContext->EnableTextures();
	}

	AddVert(rect.x, rect.y, u1, v1);
	AddVert(rect.x + rect.w, rect.y, u2, v1);
	AddVert(rect.x, rect.y + rect.h, u1, v2);
	AddVert(rect.x + rect.w, rect.y, u2, v1);
	AddVert(rect.x + rect.w, rect.y + rect.h, u2, v2);
	AddVert(rect.x, rect.y + rect.h, u1, v2);
}

void OpenGL::LoadTexture(Gwen::Texture* pTexture)
{
	const wchar_t* wFileName = pTexture->name.GetUnicode().c_str();
	FREE_IMAGE_FORMAT imageFormat = FreeImage_GetFileTypeU(wFileName);

	if (imageFormat == FIF_UNKNOWN)
	{
		imageFormat = FreeImage_GetFIFFromFilenameU(wFileName);
	}

	// Image failed to load..
	if (imageFormat == FIF_UNKNOWN)
	{
		pTexture->failed = true;
		return;
	}

	// Try to load the image..
	FIBITMAP* bits = FreeImage_LoadU(imageFormat, wFileName);

	if (!bits)
	{
		pTexture->failed = true;
		return;
	}

	// Convert to 32bit
	FIBITMAP* bits32 = FreeImage_ConvertTo32Bits(bits);
	FreeImage_Unload(bits);

	if (!bits32)
	{
		pTexture->failed = true;
		return;
	}

	// Flip
	::FreeImage_FlipVertical(bits32);
	// Create a little texture pointer..
	u32* pglTexture = new u32;
	// Sort out our GWEN texture
	pTexture->data = pglTexture;
	pTexture->width = FreeImage_GetWidth(bits32);
	pTexture->height = FreeImage_GetHeight(bits32);
#ifdef FREEIMAGE_BIGENDIAN
	u32 format = Graphics::TextureFormat::RGBA;
#else
	u32 format = Graphics::TextureFormat::BGRA;
#endif
	*pglTexture = m_ApiContext->GenerateTexture(pTexture->width, pTexture->height, format, Graphics::DataType::UBYTE, (const void*)FreeImage_GetBits(bits32));
	FreeImage_Unload(bits32);
}

void OpenGL::FreeTexture(Gwen::Texture* pTexture)
{
	u32* tex = (u32*)pTexture->data;

	if (!tex) { return; }

	m_ApiContext->DeleteTexture(tex);
	delete tex;
	pTexture->data = NULL;
}

Gwen::Color OpenGL::PixelColour(Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default)
{
	u32* tex = (u32*)pTexture->data;

	if (!tex) { return col_default; }

	unsigned char* data = m_ApiContext->GetTextureData(tex, pTexture->width, pTexture->height);
	unsigned int iOffset = (y * pTexture->width + x) * 4;
	Gwen::Color c;
	c.r = data[0 + iOffset];
	c.g = data[1 + iOffset];
	c.b = data[2 + iOffset];
	c.a = data[3 + iOffset];
	//
	// Retrieving the entire texture for a single pixel read
	// is kind of a waste - maybe cache this pointer in the texture
	// data and then release later on? It's never called during runtime
	// - only during initialization.
	//
	free(data);
	return c;
}

bool OpenGL::InitializeContext(Gwen::WindowProvider* pWindow)
{
//#ifdef _WIN32
//	HWND pHwnd = (HWND)pWindow->GetWindow();
//
//	if (!pHwnd) { return false; }
//
//	HDC hDC = GetDC(pHwnd);
//	//
//	// Set the pixel format
//	//
//	PIXELFORMATDESCRIPTOR pfd;
//	memset(&pfd, 0, sizeof(pfd));
//	pfd.nSize = sizeof(pfd);
//	pfd.nVersion = 1;
//	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
//	pfd.iPixelType = PFD_TYPE_RGBA;
//	pfd.cColorBits = 24;
//	pfd.cDepthBits = 32;
//	pfd.iLayerType = PFD_MAIN_PLANE;
//	int iFormat = ChoosePixelFormat(hDC, &pfd);
//	SetPixelFormat(hDC, iFormat, &pfd);
//	HGLRC hRC;
//	hRC = wglCreateContext(hDC);
//	wglMakeCurrent(hDC, hRC);
//	RECT r;
//
//	if (GetClientRect(pHwnd, &r))
//	{
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		glOrtho(r.left, r.right, r.bottom, r.top, -1.0, 1.0);
//		glMatrixMode(GL_MODELVIEW);
//		glViewport(0, 0, r.right - r.left, r.bottom - r.top);
//	}
//
//	m_pContext = (void*)hRC;
//	return true;
//#endif
//	return false;
	return true;
}

bool OpenGL::ShutdownContext(Gwen::WindowProvider* pWindow)
{
//#ifdef _WIN32
//	wglDeleteContext((HGLRC)m_pContext);
//	return true;
//#endif
//	return false;
	return true;
}

bool OpenGL::PresentContext(Gwen::WindowProvider* pWindow)
{
	//#ifdef _WIN32
	//	HWND pHwnd = (HWND)pWindow->GetWindow();
	//
	//	if (!pHwnd) { return false; }
	//
	//	HDC hDC = GetDC(pHwnd);
	//	SwapBuffers(hDC);
	//	return true;
	//#endif
	//	return false;
	return true;
}

bool OpenGL::ResizedContext(Gwen::WindowProvider* pWindow, int w, int h)
{
//#ifdef _WIN32
//	RECT r;
//
//	if (GetClientRect((HWND)pWindow->GetWindow(), &r))
//	{
//		glMatrixMode(GL_PROJECTION);
//		glLoadIdentity();
//		glOrtho(r.left, r.right, r.bottom, r.top, -1.0, 1.0);
//		glMatrixMode(GL_MODELVIEW);
//		glViewport(0, 0, r.right - r.left, r.bottom - r.top);
//	}
//
//	return true;
//#endif
//	return false;
	return true;
}

bool OpenGL::BeginContext(Gwen::WindowProvider* pWindow)
{
	//glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return true;
}

bool OpenGL::EndContext(Gwen::WindowProvider* pWindow)
{
	return true;
}

} }