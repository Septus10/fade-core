#include <PlatformCore/PlatformCore.hpp>

#include <Windows.h>

namespace Fade { namespace PlatformCore {

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

} }