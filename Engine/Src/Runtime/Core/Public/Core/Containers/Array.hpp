#pragma once

#include <vector>

#include <Core/CoreApi.hpp>
#include <Core/Containers/DefaultAllocator.hpp>

namespace Fade
{
	template <typename ElementType, typename Allocator = TDefaultAllocator<ElementType>>
	class FADE_CORE_API TArray : public std::vector<ElementType>
	{
	public:
		template <typename... Types>
		TArray(Types&&... a_Args) : 
			std::vector<ElementType>(std::forward<Types>(a_Args)...)
		{}
	};
}