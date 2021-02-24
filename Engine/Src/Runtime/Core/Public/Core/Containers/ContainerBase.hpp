#pragma once

#include <Core/Utility/MemoryUtil.hpp>

namespace Fade {

template <class Allocator = TDefaultAllocator<void>>
class CContainerBase
{
protected:
	Allocator m_Allocator;
};

}