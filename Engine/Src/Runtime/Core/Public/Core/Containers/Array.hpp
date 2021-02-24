#pragma once

#include <vector>

#include <Core/CoreApi.hpp>
#include <Core/definitions.hpp>
#include <Core/Utility/MemoryUtil.hpp>
#include <Core/Utility/TemplateUtil.hpp>

namespace Fade
{

template <typename ElementType, typename Allocator = TDefaultAllocator<ElementType>>
class TArray : public std::vector<ElementType, Allocator>
{
private:
	using ConstRef	= const ElementType&;
	using RvalRef	= ElementType&&;
	using Ref		= ElementType&;

public:
	template <typename... Types>
	TArray(Types&&... a_Args) : 
		std::vector<ElementType, Allocator>(std::forward<Types>(a_Args)...)
	{}

	TArray(const TArray& a_Other) = delete;
	TArray& operator=(const TArray& a_Other) = delete;

	/*
	 * Addition to array, move
	 */
	void Add(RvalRef a_Object)
	{
		push_back(Fade::Move(a_Object));
	}

	/*
	 * Addition to array, copy
	 */
	void Add(ConstRef a_Object)
	{
		push_back(a_Object);
	}

	/**
	 *
	 */
	void AddUnique(ConstRef a_Object)
	{
		// First check if we already contain an entry of the object we want to add
		for (std::vector<ElementType>::iterator it = begin(); it != end(); ++it)
		{
			if (*it == a_Object)
			{
				return;
			}
		}
		push_back(a_Object);
	}

	/**
	 * 
	 */
	void AddUnique(RvalRef a_Object)
	{
		for (std::vector<ElementType>::iterator it = begin(); it != end(); ++it)
		{
			if (*it == a_Object)
			{
				return;
			}
		}
		push_back(Fade::Move(a_Object));
	}

	/**
	 *
	 */
	void Remove(ConstRef a_Element)
	{
		std::remove(begin(), end(), a_Element);
	}

	/**
	 * 
	 */
	Fade::usize Num()
	{
		return size();
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