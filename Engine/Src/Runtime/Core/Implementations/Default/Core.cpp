#include <Core/Definitions.hpp>

#include <iostream>
#include <Core/Core.hpp>
#include <Core/Types.hpp>

#include <fstream>

namespace Fade {

SColor SColor::Red(1.f, 0.f, 0.f, 1.f);
SColor SColor::Green(0.f, 1.f, 0.f, 1.f);
SColor SColor::Blue(0.f, 0.f, 1.f, 1.f);
SColor SColor::Yellow(1.f, 1.f, 0.f, 1.f);
SColor SColor::Cyan(0.f, 1.f, 1.f, 1.f);
SColor SColor::Magenta(1.f, 0.f, 1.f, 1.f);
SColor SColor::Black(0.f, 0.f, 0.f, 1.f);
SColor SColor::White(1.f, 1.f, 1.f, 1.f);

}

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