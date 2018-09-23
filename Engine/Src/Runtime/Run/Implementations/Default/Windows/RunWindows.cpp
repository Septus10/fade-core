#include <Run/EngineLoop.hpp>

#include <Windows.h>
#include <stdio.h>
#include <iostream>

extern int Fade::main(const char* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, Fade::u32 nCmdShow);

int WINAPI WinMain(HINSTANCE hInInstance, HINSTANCE hPrevInstance, char* CmdLine, int nCmdShow)
{
//#ifdef _DEBUG
//    FILE *conin, *conout;
//    AllocConsole();
//    freopen_s(&conin, "conin$", "r", stdin);
//    freopen_s(&conout, "conout$", "w", stdout);
//    freopen_s(&conout, "conout$", "w", stderr);
//#endif

	// engine main
	return Fade::main(CmdLine, hInInstance, hPrevInstance, nCmdShow);
}