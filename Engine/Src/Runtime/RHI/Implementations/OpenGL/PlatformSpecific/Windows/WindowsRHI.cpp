#ifdef FADE_PLATFORM_WINDOWS
#include <RHI/RHI.hpp>

#include <Core/Log.hpp>

#include <PlatformCore/PlatformCore.hpp>
#include <Window/WindowsWindow.hpp>

#include "../../ApiInclude.hpp"

/// NOTE
/// So apparently we can use 1 render context for all windows as long as the pixel format of the DC is the same
/// I'll have to figure something out that nicely works with this. Re-use the same render context when the pixel format is the same
/// But I guess for now we don't have to worry about this too as the pixel format requirements are hardcoded.

// http://www.rastertek.com/gl40tut03.html
namespace Fade { inline namespace RHI {

Fade::PlatformCore::CWindowsWindow* CreateFakeWindow()
{
	static Fade::PlatformCore::SWindowSettings s_FakeWindowSettings;
	PlatformCore::CWindowsWindow* FakeWindow = new PlatformCore::CWindowsWindow();
	FakeWindow->Create(s_FakeWindowSettings);
	return FakeWindow;
}

struct STempContext
{
	STempContext()
		: m_Window(nullptr)
		, m_DeviceContext(nullptr)
		//, m_RenderContext(nullptr)
	{ }

	~STempContext()
	{
		ReleaseDC(m_Window->GetWindowHandle(), m_DeviceContext);
		//wglDeleteContext(m_RenderContext);
		m_Window->Destroy();
	}

	Fade::PlatformCore::CWindowsWindow* m_Window;
	HDC			m_DeviceContext;
}* g_TempContext = nullptr;

HGLRC g_RenderContext;

bool InitializeModernContext(
	Fade::PlatformCore::CWindow* a_Window,
	HDC& a_DeviceContext)
{
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	a_DeviceContext = GetDC((HWND)a_Window->GetOSHandle());
	if (a_DeviceContext == nullptr)
	{
		FADE_LOG(Error, WindowsRHI, "Unable to create device context for window");
		return false;
	}

	{
		const int attribList[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
			WGL_PIXEL_TYPE_ARB,		WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,		32,
			WGL_DEPTH_BITS_ARB,		24,
			WGL_STENCIL_BITS_ARB,	8,
			NULL, // End, null terminated
		};

		int PixelFormat;
		u32 NumFormats;
		int result = wglChoosePixelFormatARB(a_DeviceContext, attribList, NULL, 1, &PixelFormat, &NumFormats);
		if (result != 1)
		{
			FADE_LOG(Error, WindowsRHI, "Unable to choose pixel format: ", result);
			return false;
		}

		result = SetPixelFormat(a_DeviceContext, PixelFormat, &pfd);
		if (result != 1)
		{
			FADE_LOG(Error, WindowsRHI, "Unable to set pixel format: ", PixelFormat);
			return false;
		}
	}

	if (g_RenderContext == nullptr)
	{
		// We want at least OpenGL 4.0
		const int attribList[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
			WGL_CONTEXT_MINOR_VERSION_ARB, 0,
			0
		};

		// Create opengl rendering context 
		g_RenderContext = wglCreateContextAttribsARB(a_DeviceContext, 0, attribList);
		if (!g_RenderContext)
		{
			FADE_LOG(Error, WindowsRHI, "Unable to create render context");
			return false;
		}
	}

	int result = wglMakeCurrent(a_DeviceContext, g_RenderContext);
	if (result != 1)
	{
		FADE_LOG(Error, WindowsRHI, "Unable to set device and render context to be current:\n", GetLastError());
		return false;
	}

	return true;
}

bool InitializeAPI()
{
	HDC								DeviceCtxHandle = NULL;
	HGLRC							RenderCtxHandle = NULL;
	PlatformCore::CWindowsWindow*	FakeWindow = CreateFakeWindow();

	DeviceCtxHandle = GetDC(FakeWindow->GetWindowHandle());
	if (DeviceCtxHandle == nullptr)
	{
		return false;
	}

	PIXELFORMATDESCRIPTOR FakePFD = { 0 };
	FakePFD.nSize = sizeof(FakePFD);
	FakePFD.nVersion = 1;
	FakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	FakePFD.iPixelType = PFD_TYPE_RGBA;
	FakePFD.cColorBits = 32;
	FakePFD.cAlphaBits = 8;
	FakePFD.cDepthBits = 24;
	int FakePFDID = ChoosePixelFormat(DeviceCtxHandle, &FakePFD);
	if (FakePFDID == 0)
	{
		FADE_LOG(Error, WindowsRHI, "ChoosePixelFormat failed:\n", PlatformCore::GetLastErrorMessage());
		return false;
	}

	if (!SetPixelFormat(DeviceCtxHandle, FakePFDID, &FakePFD))
	{
		FADE_LOG(Error, WindowsRHI, "SetPixelFormat failed:");
		return false;
	}

	RenderCtxHandle = wglCreateContext(DeviceCtxHandle);

	if (RenderCtxHandle == 0)
	{
		FADE_LOG(Error, WindowsRHI, "wglCreateContext failed");
		return false;
	}

	if (!wglMakeCurrent(DeviceCtxHandle, RenderCtxHandle))
	{
		FADE_LOG(Error, WindowsRHI, "wglMakeCurrent failed");
		return false;
	}
	
	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		FADE_LOG(Error, WindowsRHI, "Unable to initialize glew:\n", glewGetErrorString(res));
		return false;
	}
	
	{
		int glMinor, glMajor;
		glGetIntegerv(GL_MAJOR_VERSION, &glMajor);
		glGetIntegerv(GL_MINOR_VERSION, &glMinor);
		FADE_LOG(Info, WindowsRHI, "Initializing opengl version: (", glMajor, ",", glMinor, ")");
	}

	g_TempContext = new STempContext();
	g_TempContext->m_Window = CreateFakeWindow();

	if (!InitializeModernContext(
		g_TempContext->m_Window,
		g_TempContext->m_DeviceContext))
	{
		FADE_LOG(Error, WindowsRHI, "Unable to create modern opengl context:\n", Fade::PlatformCore::GetLastErrorMessage());
		return false;
	}

	ReleaseDC(FakeWindow->GetWindowHandle(), DeviceCtxHandle);
	wglDeleteContext(RenderCtxHandle);
	FakeWindow->Destroy();

	g_IsRHIInitialized	= true;
	return g_IsRHIInitialized;
}

SWindowContext::SWindowContext() noexcept
	: m_ShouldDestroyWindow(false)
	, m_Window(nullptr)
	, m_DeviceContext(NULL)
	//, m_RenderContext(NULL)
{
}

SWindowContext::SWindowContext(const SWindowContext& a_Other) noexcept
	: m_ShouldDestroyWindow(a_Other.m_ShouldDestroyWindow)
	, m_Window(a_Other.m_Window)
	, m_DeviceContext(a_Other.m_DeviceContext)
	//, m_RenderContext(a_Other.m_RenderContext)
{
}

SWindowContext::SWindowContext(SWindowContext&& a_Other) noexcept
	: m_ShouldDestroyWindow(a_Other.m_ShouldDestroyWindow)
	, m_Window(a_Other.m_Window)
	, m_DeviceContext(a_Other.m_DeviceContext)
	//, m_RenderContext(a_Other.m_RenderContext)
{
	a_Other.m_Window = nullptr;
	a_Other.m_DeviceContext = nullptr;
	//a_Other.m_RenderContext = nullptr;
}

SWindowContext::~SWindowContext()
{
}

SWindowContext& SWindowContext::operator=(const SWindowContext& a_Other) noexcept
{
	m_ShouldDestroyWindow	= a_Other.m_ShouldDestroyWindow;
	m_DeviceContext			= a_Other.m_DeviceContext;
	//m_RenderContext			= a_Other.m_RenderContext;
	m_Window				= a_Other.m_Window;
	return *this;
}

SWindowContext& SWindowContext::operator=(SWindowContext&& a_Other) noexcept
{
	m_ShouldDestroyWindow	= a_Other.m_ShouldDestroyWindow;
	m_DeviceContext			= a_Other.m_DeviceContext;
	//m_RenderContext			= a_Other.m_RenderContext;
	m_Window				= a_Other.m_Window;
	a_Other.m_DeviceContext = nullptr;
	//a_Other.m_RenderContext = nullptr;
	a_Other.m_Window		= nullptr;
	return *this;
}

bool SWindowContext::Initialize(Fade::PlatformCore::CWindow* a_Window, bool a_ShouldDestroyWindow)
{
	if (!g_IsRHIInitialized)
	{
		FADE_LOG(Error, WindowsRHI, "Please initialize the api first before initializing window contexts");
		return false;
	}
	
	m_ShouldDestroyWindow = a_ShouldDestroyWindow;

	if (a_Window == nullptr)
	{
		m_Window = TSharedPtr<PlatformCore::CWindowsWindow>(CreateFakeWindow());
	}
	else
	{
		m_Window = TSharedPtr<PlatformCore::CWindowsWindow>(dynamic_cast<Fade::PlatformCore::CWindowsWindow*>(a_Window));
	}

	if (InitializeModernContext(m_Window.get(), m_DeviceContext))
	{
		if (g_TempContext != nullptr)
		{
			delete g_TempContext;
			g_TempContext = nullptr;
		}

		return true;
	}

	return false;
}

void SWindowContext::Destroy()
{
	if (m_Window != nullptr)
	{
		HWND WindowHandle = (HWND)m_Window->GetOSHandle();
		if (WindowHandle != NULL)
		{
			//wglDeleteContext(m_RenderContext);
			ReleaseDC(WindowHandle, m_DeviceContext);
			//m_RenderContext = NULL;
			m_DeviceContext = NULL;
			// If we're using a shared pointer I don't think we need to explicitly delete it. 
			// setting the m_Window to nullptr should handle it
			//if (m_ShouldDestroyWindow)
			//{
			//	delete m_Window;
			//}
			m_Window->Destroy();
		}
	}	
}

void SWindowContext::SwapBuffers() const
{
	::SwapBuffers(m_DeviceContext);

	glViewport(0, 0, m_Window->GetWidth(), m_Window->GetHeight());

	// Set the color to clear the screen to.
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	// Clear the screen and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SWindowContext::MakeCurrent() const
{
	int result = wglMakeCurrent(m_DeviceContext, g_RenderContext);
	if (result != 1)
	{
		FADE_LOG(Error, WindowsRHI, "Unable to set device and render context to be current:\n", PlatformCore::GetLastErrorMessage());
	}
}

void SWindowContext::CheckForErrors() const
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		FADE_LOG(Error, WindowsRHI, "OpenGL error: ", glewGetErrorString(error));
	}	
}

} }

#endif
