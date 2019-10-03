#include <iostream>

#include <Core/definitions.hpp>
#include <Run/EngineLoop.hpp>

#ifdef FADE_PLATFORM_WINDOWS
#include <Windows.h>
#elif FADE_PLATFORM_LINUX

#elif FADE_PLATFORM_MAC
#endif

namespace Fade
{

#ifdef FADE_PLATFORM_WINDOWS
int Fade::main(const char* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, Fade::u32 nCmdShow)
#else
int Fade::main(const char* CmdLine)
#endif
{
    CEngineLoop engineLoop;

	engineLoop.PreInitialize();

	engineLoop.Initialize();

	engineLoop.PostInitialize();

    while (engineLoop.ShouldStop())
    {
		engineLoop.Tick();
    }

	engineLoop.DeInitialize();

    return 0;
}

}