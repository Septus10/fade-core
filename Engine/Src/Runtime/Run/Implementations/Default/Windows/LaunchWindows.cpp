#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	system("PAUSE");
}

#endif