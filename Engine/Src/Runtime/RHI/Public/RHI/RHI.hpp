#pragma once

#include <RHI/RHIApi.hpp>

#include <Core/Definitions.hpp>
#include <Core/Math/Math.hpp>

#ifdef FADE_PLATFORM_WINDOWS
#include <Window/WindowsWindow.hpp>
#endif

namespace Fade { inline namespace RHI {

/**
 * Globals
 */

extern FADE_RHI_API bool g_IsRHIInitialized;

namespace TextureFormat {
extern FADE_RHI_API u32 RGBA;
extern FADE_RHI_API u32 BGRA;
extern FADE_RHI_API u32 RGB;
extern FADE_RHI_API u32 BGR;
};

namespace DataType {
extern FADE_RHI_API u32 BYTE;
extern FADE_RHI_API u32 UBYTE;
extern FADE_RHI_API u32 SHORT;
extern FADE_RHI_API u32 USHORT;
extern FADE_RHI_API u32 INT;
extern FADE_RHI_API u32 UINT;
extern FADE_RHI_API u32 FLOAT;
extern FADE_RHI_API u32 DOUBLE;
};

extern FADE_RHI_API i32 g_MaxTextureMipCount;
extern FADE_RHI_API i32 g_MaxTextureCount;

bool FADE_RHI_API InitializeAPI();

struct FADE_RHI_API SWindowContext
{
	TSharedPtr<Fade::PlatformCore::CWindow> m_Window;

#ifdef FADE_PLATFORM_WINDOWS
	HDC		m_DeviceContext;
	//HGLRC	m_RenderContext;
#elif FADE_PLATFORM_LINUX
	
#elif FADE_PLATFORM_MAC
	
#endif

	bool m_ShouldDestroyWindow;

	SWindowContext() noexcept;
	SWindowContext(const SWindowContext& a_Other) noexcept;
	SWindowContext(SWindowContext&& a_Other) noexcept;
	~SWindowContext();

	SWindowContext& operator= (const SWindowContext& a_Other) noexcept;
	SWindowContext& operator= (SWindowContext&& a_Other) noexcept;

	bool Initialize(PlatformCore::CWindow* a_Window, bool a_ShouldDestroyWindow = false);

	void Destroy();

	void SwapBuffers() const;

	void MakeCurrent() const;

	void CheckForErrors() const;
};


} }