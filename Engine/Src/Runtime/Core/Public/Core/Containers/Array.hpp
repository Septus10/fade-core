#pragma once

#include <vector>

#include <Core/CoreApi.hpp>
#include <Core/definitions.hpp>
#include <Core/Containers/DefaultAllocator.hpp>
#include <Core/Utility/TemplateUtil.hpp>

namespace Fade
{

template <typename ElementType, typename Allocator = TDefaultAllocator<ElementType>>
class TArray : public std::vector<ElementType>
{
private:
	using ConstRef	= const ElementType&;
	using RvalRef	= ElementType&&;
	using Ref		= ElementType&;

public:
	template <typename... Types>
	TArray(Types&&... a_Args) : 
		std::vector<ElementType>(std::forward<Types>(a_Args)...)
	{}

	TArray(const TArray& a_Other) = delete;
	TArray& operator=(const TArray& a_Other) = delete;

	/*
	 * Addition to array, move
	 */
	void Add(RvalRef a_Element)
	{
		push_back(Fade::Move(a_Element));
	}

	/*
	 * Addition to array, copy
	 */
	void Add(ConstRef a_Element)
	{
		push_back(a_Element);
	}

	/*
	 *
	 */
	bool IsValidIndex(Fade::u32 a_Index)
	{
		return a_Index < size() && a_Index > 0;
	}
};

}