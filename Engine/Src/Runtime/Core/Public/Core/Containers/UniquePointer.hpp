#pragma once

#include <Core/CoreApi.hpp>

#include <Core/Utility/TemplateUtil.hpp>
#include <Core/Utility/MemoryUtil.hpp>
#include <Core/Containers/SmartPointerUtil.hpp>

#include <cassert>

namespace Fade {

template <typename ObjectType, typename Deleter = TDefaultDelete<ObjectType>>
class TUniquePtr
{
public:
	using ThisType = TUniquePtr<ObjectType, Deleter>;
	using APtr = TRemoveExtentType<ObjectType>*;
	using AConstPtr = const APtr;
	using ARef = ObjectType&;
	using AConstRef = const ARef;

protected:
	APtr		m_Data;
	Deleter		m_Deleter;

public:
	/**
	 * Destructor
	 * 
	 * Uses the deleter to delete our data.
	 */
	~TUniquePtr()
	{
		m_Deleter(m_Data);
	}

	/*
	 * Default constructor
	 * 
	 * Constructs an empty unique pointer
	 */
	constexpr TUniquePtr() : m_Data(APtr())
	{ }

	/*
	 * Pointer constructor
	 * 
	 * Initializes this unique pointer object using an existing pointer.
	 */
	explicit TUniquePtr(const APtr a_Pointer) : m_Data(a_Pointer)
	{ }

	/*
	 * Nullptr constructor
	 * 
	 * Constructs an empty unique pointer
	 */
	constexpr TUniquePtr(decltype(nullptr)) : m_Data(nullptr)
	{ }

	/*
	 * Copy constructor
	 * 
	 * Explicitly deleted as we are a unique pointer
	 */
	TUniquePtr(const TUniquePtr& a_Other) = delete;

	/*
	 * Move constructor
	 * 
	 * Move the pointer from the other unique pointer to ours
	 */
	TUniquePtr(TUniquePtr&& a_Other) noexcept :
		m_Data(APtr())
	{
		Reset(a_Other.Release());
	}

	/*
	 * Move constructor from different type
	 * 
	 * Move the pointer from the other unique pointer to ours.
	 * *ONLY* if this is a related type.
	 */
	template <class OtherType, class OtherDeleter,
		typename = typename TEnableIf<TIsConvertible<OtherType*, ObjectType*>::sm_Value>::TType 
	> TUniquePtr(TUniquePtr<OtherType, OtherDeleter>&& a_Other) noexcept :
		TUniquePtr(a_Other.Release())
	{ }

	/**
	 * Move assignment operator from different type
	 * 
	 * Move the pointer from the other unique pointer to ours.
	 * *ONLY* if this is a related type.
	 */
	template <class OtherType, class OtherDeleter,
		typename = typename TEnableIf<TIsConvertible<OtherType*, ObjectType*>::sm_Value>::TType 
	> ThisType& operator=(TUniquePtr<OtherType, OtherDeleter>&& a_Other) noexcept
	{
		Reset(a_Other.Release());
		return *this;
	}
	
	/**
	 * Move assignment operator
	 * 
	 * Move the pointer from the other unique pointer to ours
	 */
	ThisType& operator=(ThisType&& a_Other) noexcept
	{
		if (this != &a_Other)
		{
			Reset(a_Other.Release());
			//GetDeleter() = std::forward<Deleter>(a_Other.GetDeleter());
		}
		return *this;
	}

	/**
	 * Nullptr assignment operator
	 * 
	 * Calls reset on this unique pointer since we are setting it to null
	 */
	ThisType& operator=(decltype(nullptr)) noexcept
	{
		Reset();
		return *this;
	}

	/**
	 * Copy assignment operator
	 * 
	 * Explicitly deleted, since we're a unique pointer
	 */
	ThisType& operator=(const ThisType& a_Other) = delete;
	
	APtr operator->() noexcept
	{
		return Get();
	}

	ARef operator*() noexcept
	{
		return *Get();
	}

	explicit operator bool() const
	{
		return IsValid();
	}

	bool operator!() const
	{
		return !IsValid();
	}

	bool operator==(AConstPtr a_Other)
	{
		return m_Data == a_Other;
	}

	template <class OtherType, class OtherDeleter,
		typename = TEnableIf<TIsConvertible<OtherType*, ObjectType*>::sm_Value>::TType>
		bool operator==(const TUniquePtr<OtherType, OtherDeleter>& a_Other)
	{
		return m_Data == a_Other.m_Data;
	}

	bool operator==(const TUniquePtr<ObjectType, Deleter>& a_Other)
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

	//====================================================================================//
	// Utility
	//====================================================================================//
	APtr Get() const noexcept
	{
		return m_Data;
	}
	
	APtr Release() noexcept
	{
		const APtr Temp = Get();
		m_Data = APtr();
		return Temp;
	}

	void Reset(AConstPtr a_NewPointer = nullptr)
	{
		const APtr Temp = Get();
		m_Data = a_NewPointer;
		if (Temp != APtr())
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
