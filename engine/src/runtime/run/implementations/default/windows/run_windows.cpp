#include <run/engine_loop.hpp>

#include <Windows.h>
#include <stdio.h>
#include <iostream>

extern int fade::main(const char* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, fade::u32 nCmdShow);

int WINAPI WinMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char* CmdLine, int nCmdShow)
{
#ifdef _DEBUG
    FILE *conin, *conout;
    AllocConsole();
    freopen_s(&conin, "conin$", "r", stdin);
    freopen_s(&conout, "conout$", "w", stdout);
    freopen_s(&conout, "conout$", "w", stderr);
#endif

	// engine main
	return fade::main(CmdLine, hInInstance, hPrevInstance, nCmdShow);
}