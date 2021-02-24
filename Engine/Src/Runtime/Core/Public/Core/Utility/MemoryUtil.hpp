#pragma once

#include <Core/CoreApi.hpp>
#include <xmemory>

namespace Fade {

template <typename ValueType>
struct TDefaultAllocator : public std::allocator<ValueType>
{

};

// Default deleter for TUniquePtr
template <typename ValueType>
struct TDefaultDelete
{
	/*
	 * Default constructor
	 */
	constexpr TDefaultDelete() noexcept = default;

	/*
	 * Copy constructor
	 */
	constexpr TDefaultDelete(const TDefaultDelete&) noexcept = default;

	/*
	 * Destructor
	 */
	~TDefaultDelete() = default;

	/*
	 * Copy assignment
	 */
	TDefaultDelete& operator=(const TDefaultDelete&) noexcept = default;

	/*
	 * Copy constructor for default deleter with other value type
	 * Note this constructor is only available if our value type pointer and the other value type pointer are convertible
	 */
	template <
		typename OtherType,
		typename = typename TEnableIf<TIsConvertible<ValueType*, OtherType*>::sm_Value>::TType
	>
		TDefaultDelete(const TDefaultDelete<OtherType>&) noexcept
	{}

	/*
	 * Parenthesis operator, takes pointer as parameter that will be deleted
	 */
	void operator()(ValueType* a_Ptr) const noexcept
	{
		static_assert(0 < sizeof(ValueType),
			"Size of ValueType is 0, \
			Unable to delete an incomplete type");
		delete a_Ptr;
	}
};

// Default array deleter for TUniquePtr
template <typename ValueType>
struct TDefaultDelete<ValueType[]>
{
	/*
	 * Default constructor
	 */
	constexpr TDefaultDelete() noexcept = default;

	/*
	 * Copy constructor
	 */
	constexpr TDefaultDelete(const TDefaultDelete&) noexcept = default;

	/*
	 * Destructor
	 */
	~TDefaultDelete() = default;

	/*
	 * Copy assignment
	 */
	TDefaultDelete& operator=(const TDefaultDelete&) noexcept = default;

	/*
	 * Copy constructor for default deleter with other value type
	 * Note this constructor is only available if our value type and the other value type are convertible
	 */
	template <
		typename OtherType,
		typename = typename TEnableIf<TIsConvertible<OtherType(*)[], ValueType(*)[]>::sm_Value>::TType
	>
		TDefaultDelete(const TDefaultDelete<OtherType[]>&) noexcept
	{}

	/*
	 * Parenthesis operator
	 * Note this function is only available if the pointers to arrays of our value type and the other value type are convertible
	 */
	template <
		typename OtherType,						// OtherType(*)[] and ValueType(*)[] are pointers-to-an-array
		typename = typename TEnableIf<TIsConvertible<OtherType(*)[], ValueType(*)[]>::sm_Value>::TType
	>
		void operator()(OtherType* a_Ptr) const noexcept
	{
		static_assert(0 < sizeof(OtherType),
			"Size of OtherType is 0, \
			Unable to delete an incomplete type");
		delete[] a_Ptr;
	}
};


}