#pragma once

#include <PlatformCore/PlatformCoreApi.hpp>
#include <PlatformCore/Application/PlatformContext.hpp>

namespace Fade { inline namespace PlatformCore {

FADE_PLATFORMCORE_API const char*	GetLastErrorMessage();

FADE_PLATFORMCORE_API CPlatformContext* CreatePlatformContext();

enum class ECursorType : unsigned char
{
	Pointer,
	ResizeUpDown,
	ResizeLeftRight,
	ResizeNESW,
	ResizeNWSE
};

FADE_PLATFORMCORE_API void SetCursorType(ECursorType a_CursorType);


} } 