#include <iostream>

#include <core/definitions.hpp>
#include <run/engine_loop.hpp>

#ifdef FADE_PLATFORM_WINDOWS
#include <Windows.h>
#elif FADE_PLATFORM_LINUX

#elif FADE_PLATFORM_MAC
#endif

namespace fade
{

#ifdef FADE_PLATFORM_WINDOWS
int fade::main(const char* CmdLine, HINSTANCE hInInstance, HINSTANCE hPrevInstance, fade::u32 nCmdShow)
#else
int fade::main(const char* CmdLine)
#endif
{
    fade::engine_loop g_engine_loop;

    g_engine_loop.pre_initialize();

    g_engine_loop.initialize();

    g_engine_loop.post_initialize();

    while (g_engine_loop.should_stop())
    {
        g_engine_loop.tick();
    }

    g_engine_loop.deinitialize();

    return 0;
}

}