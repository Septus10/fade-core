#include <iostream>
#include <vector>

#include <core.hpp>
#include <engine_loop.hpp>

#if defined(_WIN32) || defined(_WIN64)
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