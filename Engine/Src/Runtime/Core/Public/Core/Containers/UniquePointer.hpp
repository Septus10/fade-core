#pragma once

#include <Core/CoreApi.hpp>

#include <Core/Utility/TemplateUtil.hpp>
#include <Core/Utility/MemoryUtil.hpp>
#include <cassert>

namespace Fade {

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
	explicit operator bool() const
	{
		return IsValid();
	}

	bool operator!() const
	{
		return !IsValid();
	}
	
	Ref operator* ()
	{
		return *m_Data;
	}

	Ptr operator-> ()
	{
#if FADE_DEBUG
		assert(m_Data != nullptr);
#endif
		return m_Data;
	}

	bool operator==(ConstPtr a_Other)
	{
		return m_Data == a_Other;
	}

	template <class OtherType, class OtherDeleter,
		typename = TEnableIf<TIsConvertible<OtherType*, ValueType*>::sm_Value>::TType>
	bool operator==(const TUniquePtr<OtherType, OtherDeleter>& a_Other)
	{
		return m_Data == a_Other.m_Data;
	}

	bool operator==(const TUniquePtr<ValueType, Deleter>& a_Other)
	{
		return m_Data == a_Other.m_Data;
	}

	bool operator==(nullptr_t)
	{
		return m_Data == nullptr;
	}

	bool operator!=(nullptr_t)
	{
		return m_Data != nullptr;
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

template <typename ObjectType>
TUniquePtr<ObjectType> MakeUnique()
{
	return TUniquePtr<ObjectType>(new ObjectType());
}

template <typename ObjectType>
TUniquePtr<ObjectType> MakeUnique(ObjectType* a_Pointer)
{
	return TUniquePtr<ObjectType>(a_Pointer);
}

template <typename ObjectType, typename OtherType, 
	typename = TEnableIf<TIsConvertible<OtherType, ObjectType>::sm_Value>::TType>
TUniquePtr<ObjectType> MakeUnique(OtherType* a_Pointer)
{
	return TUniquePtr<ObjectType>(a_Pointer);
}

template <typename ObjectType, typename... Args>
TUniquePtr<ObjectType> MakeUnique(Args&&... a_Arguments)
{
	return TUniquePtr<ObjectType>(new ObjectType(std::forward<Args>(a_Arguments)...));
}

}
