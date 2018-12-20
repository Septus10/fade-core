#include <Core/Definitions.hpp>

#include <iostream>

void* operator new(Fade::usize a_RequestedSize) 
{
	std::cout << "Allocating memory of size: " << a_RequestedSize << "\n";
	return malloc(a_RequestedSize);
}

void operator delete(void* a_Block, Fade::usize a_BlockSize)
{
	std::cout << "Freeing memory at address: " << a_Block << " of size: " << a_BlockSize << "\n";
	return free(a_Block);
}