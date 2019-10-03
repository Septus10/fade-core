#include <editor.hpp>

#include <ECS/ECSUtil.hpp>

#include <Core/Containers/UniquePointer.hpp>
#include <PlatformCore/PlatformCore.hpp>
#include <PlatformCore/Window/Window.hpp>
#include <Rendering/Pipeline/ShaderProgram.hpp>

#include <ECS/System.hpp>
#include <ECS/Component.hpp>
#include <ECS/ComponentStore.hpp>

#define GLEW_STATIC 1

#include <GL/glew.h> 
#include <GL/wglew.h>
#include <GL/GL.h>

#include <Windows.h>
#include <iostream>

#include <Gameplay/Components/Landscape2D.hpp>
#include <Rendering/Texture.hpp>

namespace Fade {

HDC			DeviceContext;
HGLRC		RenderingContext;

unsigned int VBO, VAO, EBO;
Rendering::Pipeline::CShaderProgram Program, SpriteProgram;

float vertices[] = {
	// positions          // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {  
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

f32 g_XPosAdd = 0.f;
float g_XPosTimer = 0.f;
Rendering::CTexture texture;
Rendering::CTexture sprite;
Rendering::CTexture tilesheet;
float g_AnimTime;
float g_FrameTime = 0.25f;

int g_Column = 0;

CLandscapeChunk g_Chunk;


ETickResult CEditor::Tick(double a_DeltaTime)
{
	float fDeltaTime = float(a_DeltaTime);
	g_XPosTimer += fDeltaTime;
	g_XPosAdd = std::sin(g_XPosTimer);

	glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);

	/*Program.Use();
	
	Program.SetFloatValue(std::string("xPos"), g_XPosAdd);
	

	glBindTexture(GL_TEXTURE_2D, texture.GetTextureID());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SpriteProgram.Use();
	
	g_AnimTime += fDeltaTime;
	if (g_AnimTime > g_FrameTime)
	{
		g_Column = (g_Column + 1) % 4;
		g_AnimTime = 0.f;
	}

	const static float nx = 1.f / 9.f;
	const static float ny = 1.f / 4.f;
	
	float Column = float(g_Column);
	float Row = 0;

	SpriteProgram.SetFloatValue("nx", nx);
	SpriteProgram.SetFloatValue("ny", ny);
	SpriteProgram.SetFloatValue("Column", Column);
	SpriteProgram.SetFloatValue("Row", Row);
	
	glBindTexture(GL_TEXTURE_2D, sprite.GetTextureID());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/

	FW::FileWatcher::Get().update();

	g_Chunk.Draw(glm::mat4(0.f));

	SwapBuffers(DeviceContext);

	return ETickResult::CONTINUE;
}

void CEditor::FixedTick(double a_FixedDeltaTime)
{

}

bool CEditor::PreInitialize()
{
	m_MainWindow = GetWindow();
	
	auto& Components = Fade::ECS::Util::Registry<ECS::CComponentBase>::GetRegisteredObjects();
	std::cout << "Registered components: " << Components.size() << "\n";
	for (auto& Component : Components)
	{
		std::cout << "\t- " << Component << "\n";
		auto* Comp = Fade::ECS::Util::Registry<ECS::CComponentBase>::CreateObject(Component);
		std::cout << "\tID: " << Comp->GetID() << "\n";
	}

	return true;
}


bool CEditor::Initialize()
{
	m_CurTime = 0;

	Fade::PlatformCore::SWindowSettings WindowSettings{};
	WindowSettings.m_Title = "Fake Window";
	WindowSettings.m_HasBorder = true;
	WindowSettings.m_IsRegular = true;
	WindowSettings.m_SupportsMaximize = true;
	WindowSettings.m_SupportsMinimize = true;
	WindowSettings.m_HasSizingFrame = true;
	WindowSettings.m_PosX = 0;
	WindowSettings.m_PosY = 0;
	WindowSettings.m_Width = 1080;
	WindowSettings.m_Height = 720;

	HDC		FakeDC;
	HGLRC	FakeRC;
	TUniquePtr<Fade::PlatformCore::CWindow> FakeWindow = GetWindow();
	{
		FakeWindow->Create(WindowSettings);

		// Setup rendering context using that window
		HWND FakeWindowHandle = static_cast<HWND>(FakeWindow->GetWindowHandle());
		FakeDC = GetDC(FakeWindowHandle);

		//https://mariuszbartosik.com/opengl-4-x-initialization-in-windows-without-a-framework/
		PIXELFORMATDESCRIPTOR FakePDF = { 0 };
		FakePDF.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		FakePDF.nVersion = 1;
		FakePDF.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		FakePDF.iPixelType = PFD_TYPE_RGBA;
		FakePDF.cColorBits = 32;
		FakePDF.cAlphaBits = 8;
		FakePDF.cDepthBits = 24;

		int nPixelFormat = ChoosePixelFormat(FakeDC, &FakePDF);
		if (nPixelFormat == 0)
		{
			std::cout << "ChoosePixelFormat() failed.\n";
			return false;
		}

		if (!SetPixelFormat(FakeDC, nPixelFormat, &FakePDF))
		{
			std::cout << "SetPixelFormat() failed.\n";
			return false;
		}

		FakeRC = wglCreateContext(FakeDC);
		if (!FakeRC)
		{
			std::cout << "wglCreateContext() failed.\n";
			return false;
		}

		if (!wglMakeCurrent(FakeDC, FakeRC))
		{
			std::cout << "wglMakeCurrent() failed.\n";
			return false;
		}
	}

	// Now that we've initialized a non-modern opengl context
	// we can get the newer functionality.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "GLEW is not initialized!\n";
		return false;
	}

	if (wglewIsSupported("WGL_ARB_create_context") == 1)
	{
		WindowSettings.m_Title = "OpenGL Window";
		m_MainWindow->Create(WindowSettings);
		HWND WindowHandle = static_cast<HWND>(m_MainWindow->GetWindowHandle());

		DeviceContext = GetDC(WindowHandle);

		const int pixelAttribs[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_ALPHA_BITS_ARB, 8,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
			WGL_SAMPLES_ARB, 4,
			0
		};

		int pixelFormatID; UINT numFormats;
		bool status = bool(wglChoosePixelFormatARB(DeviceContext, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats));
		if (status == false || numFormats == 0)
		{
			std::cout << "wglChoosePixelFormatARB() failed.\n";
			return false;
		}

		PIXELFORMATDESCRIPTOR PFD;
		DescribePixelFormat(DeviceContext, pixelFormatID, sizeof(PFD), &PFD);
		if (!SetPixelFormat(DeviceContext, pixelFormatID, &PFD))
		{
			std::cout << "SetPixelFormat(). failed\n";
			return false;
		}

		int OpenGLVersion[2];
		glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
		glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);

		int  contextAttribs[] = 
		{ 
			WGL_CONTEXT_MAJOR_VERSION_ARB, OpenGLVersion[0],    
			WGL_CONTEXT_MINOR_VERSION_ARB, OpenGLVersion[1],    
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,    
			0 
		};

		RenderingContext = wglCreateContextAttribsARB(DeviceContext, nullptr, contextAttribs);
		if (!RenderingContext)
		{
			std::cout << "wglCreateContextAttribsARB() failed.\n";
			return false;
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(FakeRC);
		ReleaseDC(static_cast<HWND>(FakeWindow->GetWindowHandle()), FakeDC);
		FakeWindow->Destroy();
		if (!wglMakeCurrent(DeviceContext, RenderingContext))
		{
			std::cout << "wglMakeCurrent(). failed\n";
			return false;
		}
	}
	else
	{	//It's not possible to make a GL 3.x context. Use the old style context (GL 2.1 and before)
		m_MainWindow = Fade::Move(FakeWindow);
		RenderingContext = FakeRC;
	}

	HWND MainWindowHandle = static_cast<HWND>(m_MainWindow->GetWindowHandle());
	SetWindowText(MainWindowHandle, (LPCSTR)glGetString(GL_VERSION));
	m_MainWindow->Show();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	g_Chunk.Create(4, 4);
	g_Chunk.SetTileAtlas(Rendering::CTileAtlas(".\\Images\\TempTilemap.tga", glm::vec2(56), 4));
	g_Chunk.SetTilemap(Rendering::CTexture(".\\Images\\Tilemap.tga"));

	return true;
}

bool CEditor::PostInitialize()
{
	auto Objects = ECS::Util::Registry<ECS::CComponentBase>::GetRegisteredObjects();
	for (auto& it: Objects)
	{
		std::cout << "Registered component: " << it << "\n";
	}
	return true;
}

bool CEditor::DeInitialize()
{
	wglDeleteContext(RenderingContext);
	ReleaseDC(static_cast<HWND>(m_MainWindow->GetWindowHandle()), DeviceContext);
	m_MainWindow->Destroy();
	return true;
}

}

/** Needs to be defined out of the namespace */
Fade::TUniquePtr<Fade::CApplication> Fade::GetApplication()
{
	return Fade::MakeUnique<CEditor>();
}

