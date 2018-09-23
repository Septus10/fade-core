#pragma once

#include <Core/Containers/DefaultAllocator.hpp>

namespace Fade {

template <class Allocator = TDefaultAllocator<void>>
class CContainerBase
{
protected:
	Allocator m_Allocator;
};

}