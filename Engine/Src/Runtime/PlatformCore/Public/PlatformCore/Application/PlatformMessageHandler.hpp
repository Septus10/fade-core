#pragma once

#include <PlatformCore/PlatformCoreApi.hpp>

#include <Core/Definitions.hpp>

#include <Core/Math/Math.hpp>

#include <Core/Containers/SharedPointer.hpp>

#include <PlatformCore/Input/InputTypes.hpp>

namespace Fade { inline namespace PlatformCore {

class CWindow;

// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/Runtime/ApplicationCore/Public/GenericPlatform/GenericApplicationMessageHandler.h
class FADE_PLATFORMCORE_API IPlatformMessageHandler
{
public:
	virtual bool OnWindowResize(const u32 a_NewX, const u32 a_NewY, const u32 a_NewWidth, const u32 a_NewHeight) { return false; }
	virtual bool OnKeyChar(const u8 Character, const bool bIsRepeat) { return false; }
	virtual bool OnKeyDown(const u32 a_Key) { return false; }
	virtual bool OnKeyUp(const u32 a_Key) { return false; }
	virtual bool OnMouseMove(const u32 a_NewX, const u32 a_NewY, const u32 a_DX, const u32 a_DY) { return false; }
	virtual bool OnMouseUp(EMouseButton a_Button, Fade::Math::iVec2 a_Location) { return false; }
	virtual bool OnMouseDown(TSharedPtr<CWindow> a_Window, EMouseButton a_Button, Fade::Math::iVec2 a_Location) { return false; }
	virtual bool OnMouseDoubleClick(TSharedPtr<CWindow> a_Window, EMouseButton a_Button, Fade::Math::iVec2 a_Location) { return false; }
};

} }