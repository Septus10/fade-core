#pragma once

#include <xmemory0>

namespace Fade {

template <typename ValueType>
class FADE_CORE_API TDefaultAllocator : public std::allocator<ValueType>
{ };

}