#pragma once

#include <Core/Types.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <Core/Math/Math.hpp>

#include <Core/Math/Rect.hpp>

#include <GUI/GUIApi.hpp>

#include <PlatformCore/Window/Window.hpp>

namespace Fade { inline namespace GUI {

class IRenderer
{
public:
	virtual ~IRenderer() = default;

	virtual void Init() = 0;
	virtual void Begin(Fade::PlatformCore::CWindow* a_Window) = 0;
	virtual void End() = 0;

	// Drawing
	virtual void DrawFilledQuad(const SRect<u32>& a_Rect, const SColor& a_Color) = 0;
	//virtual void DrawTexturedQuad(const RHI::CTexture& a_Texture, const GUI::SRect& a_Rect, const Math::Vec2& a_UV1, const Math::Vec2& a_UV2) = 0;
	virtual void DrawLinedQuad(const SRect<u32>& a_Rect) = 0;
	virtual void DrawPixel(Math::Vec2 a_Location) = 0;
	//void RenderText();
};

TUniquePtr<IRenderer> FADE_GUI_API CreateRenderer();

} }