#pragma once

#include <Core/Definitions.hpp>
#include <Core/Utility/TemplateUtil.hpp>
#include <Core/Utility/MemoryUtil.hpp>

#include <memory>

namespace Fade {

// TODO: If we ever need custom functionality, implement it here
#if 0

class CRefCounterBase
{
public:
	/**
	 * Destroy managed resource
	 */
	virtual void Destroy() noexcept = 0;

	/**
	 * Destroy self
	 */
	virtual void DeleteThis() noexcept = 0;

	/**
	 * 
	 */
	usize GetUseCount()
	{

	}

	/**
	 * Increase amount of references
	 */
	void IncreaseRef()
	{

	}

	/**
	 * Increase amount of weak references
	 */
	void IncreaseWeakRef()
	{

	}

	/**
	 * Decrease amount of references
	 */
	void DecreaseRef()
	{

	}

	/**
	 * Decrease amount of weak references
	 */
	void DecreaseWeakRef()
	{

	}
};

template <class ObjectType>
class CRefCounter : public CRefCounterBase
{
public:
	explicit CRefCounter(ObjectType* a_Ptr)
		: CRefCounterBase()
		, m_Ptr(a_Ptr)
	{ }

private:
	virtual void Destroy() noexcept override
	{
		delete m_Ptr;
	}

	virtual void DeleteThis() noexcept override
	{
		delete this;
	}

	ObjectType* m_Ptr;
};

/**
 * Forward declarations
 */
template <class ObjectType>
class TSharedPointer;


template <class ObjectType, typename DeleterType = TDefaultDelete<ObjectType>>
class TSharedBase
{
public:
	using ObjectTypeInternal = TRemoveExtent<ObjectType>;

	usize UseCount() const noexcept
	{
		return m_RefCtr ? m_RefCtr->GetUseCount() : 0;
	}

	/**
	 * Deleted copy constructor and assignment operator
	 */
	TSharedBase(const TSharedBase&) = delete;
	TSharedBase& operator=(const TSharedBase&) = delete;

protected:
	ObjectTypeInternal* Get() const noexcept
	{
		return m_Ptr;
	}

	/**
	 * Default constructor and destructor
	 */
	constexpr TSharedBase() noexcept = default;
	~TSharedBase() = default;

	/**
	 * Move construct function
	 */
	template <class OtherType>
	void MoveConstructFrom(TSharedBase<OtherType>&& a_Other) noexcept
	{
		m_Ptr = a_Other.m_Ptr;
		m_RefCtr = a_Other.m_RefCtr;

		a_Other.m_Ptr = nullptr;
		a_Other.m_RefCtr = nullptr;
	}

	template <class OtherType>
	void CopyConstructFrom(const TSharedPointer<OtherType>& a_Other) noexcept
	{
		if (a_Other.m_RefCtr)
		{
			a_Other.m_RefCtr->IncreaseRef();
		}

		m_Ptr = a_Other.m_Ptr;
		m_RefCtr = a_Other.m_RefCtr;
	}
	
	template <class OtherType>
	void AliasConstructFrom(const TSharedPointer<OtherType>& a_Other, ObjectTypeInternal* a_Ptr) noexcept
	{
		if (a_Other.m_RefCtrl)
		{
			a_Other.m_RefCtrl->IncreaseRef();
		}

		m_Ptr = a_Other.m_Ptr;
		m_RefCtr = a_Other.m_RefCtr;
	}

	template <class OtherType>
	void AliasMoveConstructFrom(TSharedPointer<OtherType>&& a_Other, ObjectTypeInternal* a_Ptr) noexcept
	{
		m_Ptr = a_Ptr;
		m_RefCtr = a_Other.m_RefCtr;

		a_Other.m_Ptr = nullptr;
		a_Other.m_RefCtr = nullptr;
	}

	void DecreaseRef() noexcept
	{
		if (m_RefCtr)
		{
			m_RefCtr->DecreaseRef();
		}
	}

	void Swap(TSharedBase& a_Other) noexcept
	{
		// TODO learn how std::swap works and implement it
		std::swap(m_Ptr, a_Other.m_Ptr);
		std::swap(m_RefCtr, a_Other.m_RefCtr);
	}

	template <class OtherType>
	void WeaklyConstructFrom(const TSharedBase<OtherType>& a_Other) noexcept
	{
		if (a_Other.m_RefCtr)
		{
			m_Ptr = a_Other.m_Ptr;
			m_RefCtr = a_Other.m_RefCtr;
			m_RefCtr->IncreaseWeakRef();
		}
	}

	void DecreaseWeakRef() noexcept
	{
		if (m_RefCtr)
		{
			m_RefCtr->DecreaseWeakRef();
		}
	}

	/**
	 * The object in question
	 */
	ObjectTypeInternal* m_Ptr{ nullptr };

	/**
	 * The reference counter
	 */
	CRefCounterBase* m_RefCtr{ nullptr };
};

template <class ObjectType>
struct TTempOwner
{
	ObjectType* m_Ptr;

	explicit TTempOwner(ObjectType* const a_Ptr) noexcept
		: m_Ptr(a_Ptr)
	{ }

	TTempOwner(const TTempOwner&) = delete;
	TTempOwner& operator=(const TTempOwner&) = delete;

	~TTempOwner()
	{
		delete m_Ptr;
	}
};

template <class ObjectType, class Deleter>
struct TTempOwnerDeleter
{
	ObjectType m_Ptr;
	Deleter& m_Deleter;
	bool m_CallDeleter = true;

	/**
	 * Construct from pointer and deleter
	 */
	explicit TTempOwnerDeleter(const ObjectType a_Ptr, Deleter& a_Deleter) noexcept
		: m_Ptr(a_Ptr)
		, m_Deleter(a_Deleter)
	{ }

	/**
	 * Delete copy constructor and copy assignment operator
	 */
	TTempOwnerDeleter(const TTempOwnerDeleter&) = delete;
	TTempOwnerDeleter& operator=(const TTempOwnerDeleter&) = delete;
	
	/**
	 * Destructor
	 */
	~TTempOwnerDeleter()
	{
		if (m_CallDeleter)
		{
			m_Deleter(m_Ptr);
		}
	}
};

template <class ObjectType>
class TSharedPointer : public TSharedBase<ObjectType>
{
	using Ptr = ObjectType*;
	using ConstPtr = const Ptr;
	using Ref = ObjectType&;
	using ConstRef = const Ref;
	using ThisType = TSharedPointer<ObjectType>;
public:
	/**
	 * Construct empty shared pointer
	 */
	constexpr TSharedPointer() noexcept {}

	/**
	 * Construct empty shared pointer
	 */
	constexpr TSharedPointer(Fade::nullptr_t) noexcept {}

	/**
	 * Construct with pointer
	 */
	template <class OtherType,
		TEnableIf<TConjunctionVal<TConditional<TIsArrayVal<ObjectType>, 
	explicit TSharedPointer(OtherType* a_Object)
	{
#if _HAS_IF_CONSTEXPR
		if constexpr (TIsArrayVal<ObjectType>)
		{
			SetPointerAndDeleter(a_Object, TDefaultDelete<OtherType>{});
		}
		else
		{
			TTempOwner<OtherType> Owner(a_Object);
			SetAndEnableShared(Owner.m_Ptr, new CRefCounter<OtherType>(Owner.m_Ptr));
			Owner.m_Ptr = nullptr;
		}
#else
		SetPointer(a_Object, TIsArray<ObjectType>{});
#endif		
	}

	/**
	 * Construct with pointer and deleter
	 */
	template <class OtherType, class DeleterType>
	TSharedPointer(OtherType * a_Ptr, DeleterType a_Deleter)
	{
		SetPointerAndDeleter(a_Ptr, Move(a_Deleter));
	}
private:

	template <class PointerType>
	void SetPointer(const PointerType a_Ptr, TTrueType)
	{

	}

	template <class PointerType>
	void SetPointer(const PointerType a_Ptr, TFalseType)
	{

	}

	template <class PointerOrNullptr, class Deleter>
	void SetPointerAndDeleter(const PointerOrNullptr a_Ptr, Deleter a_Deleter)
	{

	}

	template <class OtherType>
	void SetAndEnableShared(ObjectTypeInternal* const a_Ptr, CRefCounterBase* const a_RefCtr) noexcept
	{
		m_Ptr = a_Ptr;
		m_RefCtr = a_RefCtr;
	}

private:
	
};

template <typename ObjectType>
TSharedPointer<ObjectType>&& MakeShared()
{
#ifdef _DEBUG
	static_assert(!TIsPointer<ObjectType>::sm_Value && !TIsReference<ObjectType>::sm_Value, "Please make sure the type passed as template isn't a pointer or reference");
#endif
	return TSharedPointer(new ObjectType);
}

template <typename ObjectType, typename... Args>
TSharedPointer<ObjectType>&& MakeShared(Args&&... a_Arguments)
{
#ifdef _DEBUG
	static_assert(!TIsPointer<ObjectType>::sm_Value && !TIsReference<ObjectType>::sm_Value, "Please make sure the type passed as template isn't a pointer or reference");
#endif
	return TSharedPointer(new ObjectType(Forward(a_Arguments)));
}

template <typename ObjectType>
TSharedPointer<ObjectType>&& MakeShared(ObjectType* a_Object)
{
#ifdef _DEBUG
	static_assert(!TIsPointer<ObjectType>::sm_Value && !TIsReference<ObjectType>::sm_Value, "Please make sure the type passed as template isn't a pointer or reference");
#endif
	return TSharedPointer(a_Object);
}

#endif

template <typename T>
using TSharedPtr = std::shared_ptr<T>;

template <typename Type>
bool IsValid(const TSharedPtr<Type>& a_SharedPtr)
{
	return a_SharedPtr.get() != nullptr;
}

}