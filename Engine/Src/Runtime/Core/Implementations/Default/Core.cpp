#include <Core/Definitions.hpp>

#include <iostream>
#include <Core/Core.hpp>

#include <fstream>

void* operator new(Fade::usize a_RequestedSize) 
{
	std::cout << "Allocating memory of size: " << a_RequestedSize << "\n";
	void* address = malloc(a_RequestedSize);
	std::cout << "Pointer: " << reinterpret_cast<Fade::usize>(address) << "\n";
	return address;
}

void operator delete(void* a_Block, Fade::usize a_BlockSize)
{
	std::cout << "Freeing memory at address: " << a_Block << " of size: " << a_BlockSize << "\n";
	return free(a_Block);
}