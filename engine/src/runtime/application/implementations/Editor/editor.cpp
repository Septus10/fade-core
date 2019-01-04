#include <editor.hpp>

#include <Core/Containers/UniquePointer.hpp>
#include <PlatformCore/PlatformCore.hpp>
#include <PlatformCore/Window/Window.hpp>
#include <Rendering/Pipeline/ShaderProgram.hpp>

#define GLEW_STATIC 1

#include <GL/glew.h> 
#include <GL/wglew.h>
#include <GL/GL.h>

#include <Windows.h>
#include <iostream>

namespace Fade {

HDC			DeviceContext;
HGLRC		RenderingContext;

unsigned int VBO, VAO, EBO;
Rendering::Pipeline::CShaderProgram Program;

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

f32 g_XPosAdd = 0.f;
float g_XPosTimer = 0.f;

ETickResult CEditor::Tick(double a_DeltaTime)
{
	g_XPosTimer += a_DeltaTime;
	g_XPosAdd = std::sin(g_XPosTimer);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	glDrawArrays(GL_TRIANGLES, 0, 3);

	SwapBuffers(DeviceContext);

	return ETickResult::CONTINUE;
}

void CEditor::FixedTick(double a_FixedDeltaTime)
{

}

bool CEditor::PreInitialize()
{
	m_MainWindow = GetWindow();
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
		bool status = wglChoosePixelFormatARB(DeviceContext, pixelAttribs, NULL, 1, &pixelFormatID, &numFormats);
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

	Program.LoadShaderProgram(".\\Shaders\\Default\\");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0); 

	return true;
}

bool CEditor::PostInitialize()
{
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

