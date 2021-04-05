#include <PlatformCore/PlatformCore.hpp>

// Platform includes
#include <windowsx.h>
#include <Windows.h>

// STL includes
#include <iostream>

#include <Window/WindowsWindow.hpp>
#include <Application/WindowsContext.hpp>

#include <Core/Log.hpp>

namespace Fade { inline namespace PlatformCore {

LPTSTR UTF16ToUTF8(wchar_t* InString)
{
	const int size = ::WideCharToMultiByte(CP_UTF8, 0, InString, -1, NULL, 0, 0, NULL);

	char* buf = new char[size];
	::WideCharToMultiByte(CP_UTF8, 0, InString, -1, buf, size, 0, NULL);

	return buf;
}

const char* Fade::PlatformCore::GetLastErrorMessage()
{
	DWORD ErrorID = ::GetLastError();
	wchar_t buf[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, ErrorID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	return UTF16ToUTF8(buf);
}

FADE_PLATFORMCORE_API CPlatformContext* CreatePlatformContext()
{
	HICON IconHandle = NULL; // LoadIcon(Windows::g_InstanceHandle, MAKEINTRESOURCE(GetAppIcon()));
	if (IconHandle == NULL)
	{
		IconHandle = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
	}

	return CWindowsContext::CreateWindowsContext(Windows::g_InstanceHandle, IconHandle);
}

FADE_PLATFORMCORE_API void SetCursorType(ECursorType a_CursorType)
{
	HCURSOR Cursor = nullptr;
	switch (a_CursorType)
	{
		case ECursorType::Pointer:
			Cursor = LoadCursor(NULL, IDC_ARROW);
			break;
		case ECursorType::ResizeUpDown:
			Cursor = LoadCursor(NULL, IDC_SIZENS);
			break;
		case ECursorType::ResizeLeftRight:
			Cursor = LoadCursor(NULL, IDC_SIZEWE);
			break;
		case ECursorType::ResizeNESW:
			Cursor = LoadCursor(NULL, IDC_SIZENESW);
			break;
		case ECursorType::ResizeNWSE:
			Cursor = LoadCursor(NULL, IDC_SIZENWSE);
			break;
	}

	if (Cursor != nullptr)
	{
		SetCursor(Cursor);
	}
#if FADE_DEBUG
	else
	{
		FADE_LOG(Error, PlatformWindows, "Unable to set cursor:\n", PlatformCore::GetLastErrorMessage());
	}
#endif
}

} }