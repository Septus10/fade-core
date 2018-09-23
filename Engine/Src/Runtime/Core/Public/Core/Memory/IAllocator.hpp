#pragma once

#include <Core/Definitions.hpp>

namespace Fade {

class IAllocator
{
public:
	virtual void* Allocate(usize a_Size) = 0;
	virtual void Free(void* a_Block, usize a_Size) = 0;
};

}