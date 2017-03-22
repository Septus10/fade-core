#ifdef FADE_PLATFORM_WINDOWS
#include <run/engine_loop.hpp>

#include <Windows.h>

#ifdef _DEBUG
int main(int argc, char** argv)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	// parse arguments
	

	// engine main
	return fade::main();
}

#endif