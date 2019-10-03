#pragma once

#include <Core/CoreApi.hpp>

#include <Core/Utility/TemplateUtil.hpp>
#include <cassert>

namespace Fade {

// Default deleter for TUniquePtr
template <typename ValueType>
struct FADE_CORE_API TDefaultDelete
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
struct FADE_CORE_API TDefaultDelete<ValueType[]>
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

template <typename ValueType, typename Deleter = TDefaultDelete<ValueType>>
class TUniquePtr
{
public:
	using Ptr = ValueType*;
	using ConstPtr = const Ptr;
	using Ref = ValueType&;
	using ConstRef = const Ref;
	using ThisType = TUniquePtr<ValueType, Deleter>;

private:
	Ptr			m_Data;
	Deleter		m_Deleter;

public:
	//====================================================================================//
	// Constructors & Destructor
	//====================================================================================//
	/*
	 * Default CTor
	 */
	constexpr TUniquePtr() : m_Data(Ptr())
	{ }

	/*
	 * CTor with pointer parameter
	 */
	explicit TUniquePtr(const Ptr a_Pointer) : m_Data(a_Pointer)
	{ }

	/*
	 * CTor with nullptr param
	 */
	constexpr TUniquePtr(decltype(nullptr)) : m_Data(nullptr)
	{ }

	/*
	 * Copy CTors are deleted, we don't want to copy around unique pointers
	 */
	TUniquePtr(const TUniquePtr& a_Other) = delete;

	/*
	 * Move CTor get the pointer from the other unique pointer and invalidate theirs
	 */
	TUniquePtr(TUniquePtr&& a_Other) noexcept :
		m_Data(Ptr())
	{
		Reset(a_Other.Release());
	}

	/*
	 * CTor from different type
	 */
	template <class OtherType, class OtherDeleter,
		typename = typename TEnableIf<TIsConvertible<OtherType*, ValueType*>::sm_Value>::TType 
	> TUniquePtr(TUniquePtr<OtherType, OtherDeleter>&& a_Other) noexcept :
		TUniquePtr(a_Other.Release())
	{ }

	~TUniquePtr()
	{
		m_Deleter(m_Data);
	}

	//====================================================================================//
	// Operators
	//====================================================================================//
	Ref operator* ()
	{
		return *m_Data;
	}

	Ptr operator-> ()
	{
		return m_Data;
	}

	bool operator==(ConstPtr a_Other)
	{
		return m_Data == a_Other;
	}

	template <class OtherType, class OtherDeleter,
		typename = typename TEnableIf<TIsConvertible<OtherType*, ValueType*>::sm_Value>::TType 
	> ThisType& operator=(TUniquePtr<OtherType, OtherDeleter>&& a_Other) noexcept
	{
		Reset(a_Other.Release());
		return *this;
	}
	
	ThisType& operator=(ThisType&& a_Other) noexcept
	{
		if (this != &a_Other)
		{
			Reset(a_Other.Release());
			//GetDeleter() = std::forward<Deleter>(a_Other.GetDeleter());
		}
		return *this;
	}


	ThisType& operator=(decltype(nullptr)) noexcept
	{
		Reset();
		return *this;
	}

	ThisType& operator=(const ThisType& a_Other) = delete;
	
	//====================================================================================//
	// Utility
	//====================================================================================//
	Ptr Get() const noexcept
	{
		return m_Data;
	}

	Ptr Release() noexcept
	{
		const Ptr Temp = Get();
		m_Data = Ptr();
		return Temp;
	}

	void Reset(ConstPtr a_NewPointer = nullptr)
	{
		const Ptr Temp = Get();
		m_Data = a_NewPointer;
		if (Temp != Ptr())
		{
			m_Deleter(Temp);
		}
	}

	bool IsValid() const noexcept
	{
		return m_Data != nullptr;
	}

	Deleter* GetDeleter() const noexcept
	{
		return m_Deleter;
	}
};

template <typename T>
TUniquePtr<T> MakeUnique()
{
	return TUniquePtr<T>(new T());
}

template <typename T>
TUniquePtr<T> MakeUnique(T* a_Pointer)
{
	return TUniquePtr<T>(a_Pointer);
}

template <typename T, typename... Args>
TUniquePtr<T> MakeUnique(Args&&... a_Arguments)
{
	return TUniquePtr<T>(new T(std::forward(a_Arguments)));
}

}
