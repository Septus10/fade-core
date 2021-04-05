#pragma once

#include <Core/Core.hpp>
#include <Core/Definitions.hpp>
#include <Core/Utility/TemplateUtil.hpp>
#include <Core/Utility/MemoryUtil.hpp>

#include <Core/Containers/SmartPointerUtil.hpp>

#include <memory>
#include <cassert>

namespace Fade {

// TODO: If we ever need custom functionality, implement it here
#if 1

enum class EReferenceMode
{
	NotThreadSafe,
	ThreadSafe
};

template <EReferenceMode Mode>
struct TReferenceControllerOperations;

/**
 * Reference controller base class
 *
 * Contains the counters
 */
class IReferenceControllerBase
{
public:
	/**
	 * This function can be overridden to destroy the reference we're pointing towards.
	 */
	virtual void DeletePointer() noexcept = 0;

	/**
	 * This function can be overridden to destroy this entire object, should be done in a derived class.
	 */
	virtual void DeleteSelf() noexcept = 0;

private:
	/**
	 * Number of shared references of our controlled reference.
	 *
	 * Once this reaches zero we can safely delete our pointer.
	 */
	u32 m_SharedRefCount = 1;

	/**
	 * Total number of weak references of our controlled reference.
	 *
	 * All shared references count as 1 weak reference, 
	 * thus once this is created we set the weak ref count equal to that of shared references
	 */
	u32 m_WeakRefCount = 1;

	/** make the operations implementations a friend class so they can access our counters */
	friend struct TReferenceControllerOperations<EReferenceMode::NotThreadSafe>;
	friend struct TReferenceControllerOperations<EReferenceMode::ThreadSafe>;
};

/**
 * Inline reference controller
 * 
 * Used to create an object that holds a pointer + the reference count in one heap allocation
 */
template <class ObjectType>
class TInlineReferenceController : public IReferenceControllerBase
{
public:
	template <typename... ArgTypes>
	explicit TInlineReferenceController(ArgTypes... a_Args)
	{
		new ((void*)&m_Storage) ObjectType(Fade::Forward<ArgTypes>(a_Args)...);
	}

	ObjectType m_Storage;

private:
	void DeletePointer() noexcept override
	{
		DestroyInPlace(m_Storage);
	}

	void DeleteSelf() noexcept override
	{
		delete this;
	}
};

/**
 * Reference controller class
 *
 * Keeps track of the count
 */
template <class ObjectType, class Deleter = TDefaultDelete<ObjectType>>
class TReferenceController : public IReferenceControllerBase
{
public:
	void DeletePointer() noexcept override
	{
		assert(m_Ptr != nullptr);
		m_Deleter(m_Ptr);
		m_Ptr = nullptr;
	}

	void DeleteSelf() noexcept override
	{
		delete this;
	}

private:
	Deleter m_Deleter;
	ObjectType* m_Ptr;
};

template <>
struct TReferenceControllerOperations<EReferenceMode::ThreadSafe>
{
	static inline void IncreaseSharedRefCount(IReferenceControllerBase* a_ReferenceController) noexcept
	{

	}

	static inline bool IncreaseSharedRefCountNonZero(IReferenceControllerBase* a_ReferenceController) noexcept
	{

	}

	static inline void DecreaseSharedRefCount(IReferenceControllerBase* a_ReferenceController) noexcept
	{

	}

	static inline void IncreaseWeakRefCount(IReferenceControllerBase* a_ReferenceController) noexcept
	{

	}

	static inline void DecreaseWeakRefCount(IReferenceControllerBase* a_ReferenceController) noexcept
	{

	}
};

template <>
struct TReferenceControllerOperations<EReferenceMode::NotThreadSafe>
{
	static inline void IncreaseSharedRefCount(IReferenceControllerBase* a_ReferenceController) noexcept
	{
		if (a_ReferenceController != nullptr)
		{
			++a_ReferenceController->m_SharedRefCount;
		}
	}

	static inline bool IncreaseSharedRefCountNonZero(IReferenceControllerBase* a_ReferenceController) noexcept
	{
		if (a_ReferenceController != nullptr)
		{
			if (a_ReferenceController->m_SharedRefCount == 0)
			{
				return false;
			}

			++a_ReferenceController->m_SharedRefCount;
			return true;
		}
	}

	static inline void DecreaseSharedRefCount(IReferenceControllerBase* a_ReferenceController) noexcept
	{
		if (a_ReferenceController != nullptr)
		{
			assert(a_ReferenceController->m_SharedRefCount > 0);
			if (--a_ReferenceController->m_SharedRefCount == 0)
			{
				a_ReferenceController->DeletePointer();
				DecreaseWeakRefCount(a_ReferenceController);
			}
		}
	}

	static inline void IncreaseWeakRefCount(IReferenceControllerBase* a_ReferenceController) noexcept
	{
		if (a_ReferenceController != nullptr)
		{
			++a_ReferenceController->m_WeakRefCount;
		}
	}

	static inline void DecreaseWeakRefCount(IReferenceControllerBase* a_ReferenceController) noexcept
	{
		if (a_ReferenceController != nullptr)
		{
			assert(a_ReferenceController->m_WeakRefCount > 0);
			if (--a_ReferenceController->m_WeakRefCount == 0)
			{
				a_ReferenceController->DeleteSelf();
			}
		}
	}
};

// Forward declaration
template <class ObjectType, EReferenceMode Mode = EReferenceMode::NotThreadSafe>
class TSharedPtr;
template <typename ObjectType, EReferenceMode Mode = EReferenceMode::NotThreadSafe>
class TWeakPtr;

template <class ObjectType, EReferenceMode Mode = EReferenceMode::NotThreadSafe>
class TRefPointerBase
{
public:
	using APtr = TRemoveExtentType<ObjectType>*;
	using AConstPtr = const APtr;
	using ARef = ObjectType&;
	using AConstRef = const ARef;

	APtr Get() const noexcept
	{
		return m_Ptr;
	}

	bool IsValid() const noexcept
	{
		return m_Ptr != nullptr;
	}

	explicit operator bool() const noexcept
	{
		return IsValid();
	}

	bool operator!() const noexcept
	{
		return !IsValid();
	}

	ARef operator* () const noexcept
	{
		return *Get();
	}

	APtr operator-> () const noexcept
	{
#if FADE_DEBUG
		assert(m_Ptr != nullptr);
#endif
		return Get;
	}

	bool operator==(AConstPtr a_Other) const noexcept
	{
		return m_Ptr == a_Other;
	}

	bool operator==(nullptr_t)  const noexcept
	{
		return m_Ptr == nullptr;
	}

	bool operator!=(nullptr_t) const noexcept
	{
		return m_Ptr != nullptr;
	}

protected:
	using TControllerOps = TReferenceControllerOperations<Mode>;

	template <typename PointerType>
	void CopyConstructFrom(const TSharedPtr<PointerType>& a_Other) noexcept
	{
		TReferenceControllerOperations<Mode>::IncreaseSharedRefCount(a_Other.m_Controller);
		m_Ptr			= a_Other.m_Ptr;
		m_Controller	= a_Other.m_Controller;
	}

	template <typename PointerType>
	void MoveConstructFrom(TSharedPtr<PointerType>&& a_Other) noexcept
	{
		m_Ptr			= a_Other.m_Ptr;
		m_Controller	= a_Other.m_Controller;

		a_Other.m_Ptr			= nullptr;
		a_Other.m_Controller	= nullptr;
	}

	template <class PointerType>
	void AliasConstructFrom(const TSharedPtr<PointerType>& a_Other, APtr a_Ptr) noexcept
	{
		TReferenceControllerOperations<Mode>::IncreaseSharedRefCount(a_Other.m_Controller);

		m_Ptr			= a_Ptr;
		m_Controller	= a_Other.m_Controller;
	}

	template <class PointerType>
	void AliasMoveConstructFrom(TSharedPtr<PointerType>&& a_Other, APtr a_Ptr) noexcept
	{
		m_Ptr			= a_Ptr;
		m_Controller	= a_Other.m_Controller;

		a_Other.m_Ptr			= nullptr;
		a_Other.m_Controller	= nullptr;
	}

	template <typename PointerType>
	void Swap(TSharedPtr<PointerType>& a_Other) noexcept
	{
		Fade::Swap(m_Ptr, a_Other.m_Ptr);
		Fade::Swap(m_Controller, a_Other.m_Controller);
	}

	template <typename PointerType>
	void InitFromPtrAndCtrl(PointerType* const a_Pointer, IReferenceControllerBase* const a_ReferenceController) noexcept
	{
		m_Ptr			= a_Pointer;
		m_Controller	= a_ReferenceController;
	}

	template <class PointerType, EReferenceMode Mode>
	friend class TWeakPtr;

	template <typename PointerType>
	bool ConstructFromWeak(const TWeakPtr<PointerType>& a_Other) noexcept
	{
		if (TControllerOps::IncreaseSharedRefCountNonZero(a_Other.m_Controller))
		{
			m_Ptr			= a_Other.m_Ptr;
			m_Controller	= a_Other.m_Controller;
			return true;
		}

		return false;
	}

	template <typename PointerType>
	void WeaklyConstructFrom(const TRefPointerBase<PointerType>& a_Other) noexcept
	{
		if (a_Other.m_Controller != nullptr)
		{
			TControllerOps::IncreaseWeakRefCount(a_Other.m_Controller);
			m_Ptr			= a_Other.m_Ptr;
			m_Controller	= a_Other.m_Controller;
		}
	}

	void IncreaseSharedReferences() const noexcept
	{
		if (m_Controller != nullptr)
		{
			TControllerOps::IncreaseSharedRefCount(m_Controller);
		}
	}

	void DecreaseSharedReferences() const noexcept
	{
		if (m_Controller != nullptr)
		{
			TControllerOps::DecreaseSharedRefCount(m_Controller);
		}
	}

	void IncreaseWeakReferences() const noexcept
	{
		if (m_Controller != nullptr)
		{
			TControllerOps::IncreaseWeakRefCount(m_Controller);
		}
	}

	void DecreaseWeakReferences() const noexcept
	{
		if (m_Controller != nullptr)
		{
			TControllerOps::DecreaseWeakRefCount(m_Controller);
		}
	}

protected:
	APtr m_Ptr								= nullptr;
	IReferenceControllerBase* m_Controller	= nullptr;

	template <class OtherType, EReferenceMode Mode>
	friend class TRefPointerBase;

	friend TSharedPtr<ObjectType>;
};

template <class ObjectType>
struct TTemporaryOwner 
{
	ObjectType* m_Ptr;

	explicit TTemporaryOwner(ObjectType* const a_Ptr) noexcept 
		: m_Ptr(a_Ptr)
	{ }

	// copy constructor and assignment operator are explicitly deleted
	TTemporaryOwner(const TTemporaryOwner&) = delete;
	TTemporaryOwner& operator=(const TTemporaryOwner&) = delete;

	// Constructor with regular delete
	~TTemporaryOwner()
	{
		delete m_Ptr;
	}
};

template <class PointerType, class Deleter>
struct TTemporaryOwnerWithDeleter 
{
	PointerType m_Ptr;
	Deleter& m_Deleter;
	bool m_CallDeleter = true;

	explicit TTemporaryOwnerWithDeleter(const PointerType a_Ptr, Deleter& a_Deleter) noexcept
		: m_Ptr(a_Ptr)
		, m_Deleter(a_Deleter)
	{
	}

	// Copy constructor and assignment operator are explicitly deleted
	TTemporaryOwnerWithDeleter(const TTemporaryOwnerWithDeleter&) = delete;
	TTemporaryOwnerWithDeleter& operator=(const TTemporaryOwnerWithDeleter&) = delete;
	
	// Destructor with deleter
	~TTemporaryOwnerWithDeleter()
	{
		if (m_CallDeleter)
		{
			m_Deleter(m_Ptr);
		}
	}
};

template <class ObjectType, EReferenceMode Mode>
class TSharedPtr : public TRefPointerBase<ObjectType, Mode>
{
	using Super = TRefPointerBase<ObjectType, Mode>;
public:
	/**
	 * Default constructor
	 *
	 * Constructs an empty shared pointer of our object type.
	 */
	constexpr TSharedPtr() noexcept = default;

	/**
	 * Nullptr constructor
	 *
	 * Constructs an empty shared pointer of our object type.
	 */
	constexpr TSharedPtr(std::nullptr_t) noexcept
	{ }

	/**
	 * Raw pointer constructor of same type
	 */
	TSharedPtr(ObjectType* a_Pointer)
	{
		m_Ptr = a_Pointer;
		m_Controller = new TReferenceController<ObjectType>();
	}

	/**
	 * Raw pointer constructor
	 *
	 * Constructs a shared pointer from a raw pointer.
	 * This constructor creates and initializes the reference counter.
	 */
	template <class PointerType>
	TSharedPtr(PointerType* a_Pointer)
	{
		static_assert(TIsBaseOf<ObjectType, PointerType>::sm_Value, "Unable to instantiate shared pointer with unrelated pointer");
		m_Ptr			= a_Pointer;
		m_Controller	= new TReferenceController<ObjectType>();
	}

	/**
	 * 
	 */
	TSharedPtr(const TSharedPtr& a_Other)
	{
		CopyConstructFrom(a_Other);
	}

	/**
	 * Copy constructor
	 *
	 * Constructs a shared pointer object from an existing one of a connected type.
	 * This constructor increases the reference counter.
	 */
	template <class PointerType>
	TSharedPtr(const TSharedPtr<PointerType>& a_Other)
	{
		static_assert(TIsBaseOf<ObjectType, PointerType>::sm_Value, "Unable to copy construct a shared pointer from another unrelated shared pointer");
		CopyConstructFrom(a_Other);
	}

	/**
	 * Move constructor from same type
	 */
	TSharedPtr(TSharedPtr&& a_Other)
	{
		MoveConstructFrom(Fade::Move(a_Other));
	}

	/**
	 * Move constructor from other type
	 *
	 *
	 */
	template <class PointerType>
	TSharedPtr(TSharedPtr<PointerType>&& a_Other)
	{
		static_assert(TIsBaseOf<ObjectType, PointerType>::sm_Value, "Unable to move construct a shared pointer from another unrelated shared pointer");
		MoveConstructFrom(Fade::Move(a_Other));
	}

	/**
	 * Alias copy constructor
	 * 
	 * 
	 */
	template <class PointerType>
	TSharedPtr(const TSharedPtr<PointerType>& a_Other, APtr a_Ptr) noexcept
	{
		AliasConstructFrom(a_Other, a_Ptr);
	}

	/**
	 * Alias move constructor
	 */
	template <class PointerType>
	TSharedPtr(TSharedPtr<PointerType>&& a_Other, APtr a_Ptr) noexcept
	{
		AliasMoveConstructFrom(a_Other, a_Ptr);
	}

	/**
	 * Copy assignment from same type
	 */
	TSharedPtr& operator= (const TSharedPtr& a_Other) noexcept
	{
		TSharedPtr(a_Other).Swap(*this);
		return *this;
	}

	/**
	 * Copy assignment from other type
	 *
	 * 
	 */
	template <class PointerType>
	TSharedPtr& operator= (const TSharedPtr<PointerType>& a_Other) noexcept
	{
		static_assert(TIsBaseOf<ObjectType, PointerType>::sm_Value, "Unable to copy assign a shared pointer from another unrelated shared pointer");
		TSharedPtr(a_Other).Swap(*this);
		return *this;
	}

	/**
	 * Move assignment from same type
	 * 
	 * We consume the other shared pointer
	 */
	TSharedPtr& operator= (TSharedPtr&& a_Other) noexcept
	{
		TSharedPtr(Fade::Move(a_Other)).Swap(*this);
		return *this;
	}

	/**
	 * Move assignment from other type
	 *
	 * We consume the other shared pointer
	 */
	template <class PointerType>
	TSharedPtr& operator= (TSharedPtr<PointerType>&& a_Other) noexcept
	{
		static_assert(TIsBaseOf<ObjectType, PointerType>::sm_Value, "Unable to move assign a shared pointer from another unrelated shared pointer");
		TSharedPtr(Fade::Move(a_Other)).Swap(*this);
		return *this;
	}

	/**
	 * Release resource and convert to empty shared_ptr object
	 */
	void Reset() noexcept
	{
		TSharedPtr().Swap(*this);
	}

	/**
	 * Release resource and take ownership of the new pointer
	 */
	template <class PointerType>
	void Reset(PointerType* a_Ptr)
	{
		TSharedPtr(a_Ptr).Swap(*this);
	}

	using TRefPointerBase<ObjectType>::Get;

	/**
	 * Operators
	 */
	template <typename PointerType = ObjectType, typename = TEnableIf<!TIsArray<ObjectType>::sm_Value>::TType>
	FADE_NODISCARD PointerType* operator->() const noexcept
	{
		return Get();
	}

	template <typename PointerType = ObjectType, typename = TEnableIf<!TDisjunctionValue<TIsArray<ObjectType>, TIsVoid<ObjectType>>>::TType>
	FADE_NODISCARD ObjectType& operator*() const noexcept
	{
		return *Get();
	}

private:
	template <class PointerType, class Deleter>
	void SetPointerDeleter(const PointerType a_Ptr, Deleter a_Deleter)
	{
		TTemporaryOwnerWithDeleter<PointerType, Deleter> Owner(a_Ptr, a_Deleter);
		EnableSharedFromPtrAndCtrl(
			Owner.m_Ptr, 
			new TReferenceController<PointerType, Deleter>(
				Owner.m_Ptr,
				Fade::Move(a_Deleter)
			)
		);
		Owner.m_CallDeleter = false;
	}

	template <class ObjectType, class... Types>
	friend TSharedPtr<ObjectType> MakeShared(Types&&... a_Args);

	template <class PointerType>
	void EnableSharedFromPtrAndCtrl(ObjectType* const a_Ptr, IReferenceControllerBase* const a_Controller) noexcept
	{
		m_Ptr			= a_Ptr;
		m_Controller	= a_Controller;
	}

	template <class PointerType>
	void EnableSharedFromPtrAndCtrl(Fade::nullptr_t, IReferenceControllerBase* const a_Controller) noexcept
	{
		m_Ptr			= nullptr;
		m_Controller	= a_Controller;
	}
};

/**
 * Shared pointer operators
 */
template <class A, class B>
bool operator== (const TSharedPtr<A>& a_Lhs, const TSharedPtr<B>& a_Rhs) noexcept
{
	return a_Lhs.Get() == a_Rhs.Get();
}

template <class A, class B>
bool operator== (const TSharedPtr<A>& a_Lhs, Fade::nullptr_t) noexcept
{
	return a_Lhs.Get() == nullptr;
}

template <class A, class B>
bool operator== (Fade::nullptr_t, const TSharedPtr<A>& a_Rhs) noexcept
{
	return nullptr == a_Rhs.Get();
}

template <class A, class B>
bool operator!= (const TSharedPtr<A>& a_Lhs, const TSharedPtr<B>& a_Rhs) noexcept
{
	return a_Lhs.Get() != a_Rhs.Get();
}

template <class A, class B>
bool operator!= (const TSharedPtr<A>& a_Lhs, Fade::nullptr_t) noexcept
{
	return a_Lhs.Get() != nullptr;
}

template <class A, class B>
bool operator!= (Fade::nullptr_t, const TSharedPtr<B>& a_Rhs) noexcept
{
	return nullptr != a_Rhs.Get();
}

template <class A, class B>
bool operator< (const TSharedPtr<A>& a_Lhs, const TSharedPtr<B>& a_Rhs) noexcept
{
	return a_Lhs.Get() < a_Rhs.Get();
}

template <class A, class B>
bool operator> (const TSharedPtr<A>& a_Lhs, const TSharedPtr<B>& a_Rhs) noexcept
{
	return a_Lhs.Get() > a_Rhs.Get();
}

template <class A, class B>
bool operator<= (const TSharedPtr<A>& a_Lhs, const TSharedPtr<B>& a_Rhs) noexcept
{
	return a_Lhs.Get() <= a_Rhs.Get();
}

template <class A, class B>
bool operator>= (const TSharedPtr<A>& a_Lhs, const TSharedPtr<B>& a_Rhs) noexcept
{
	return a_Lhs.Get() >= a_Rhs.Get();
}

/**
 * Dynamic pointer cast to downcast to a derived class
 * This requires RTTI
 * 
 * This function invokes a copy construction
 */
template <class To, class From>
FADE_NODISCARD TSharedPtr<To> DynamicPointerCast(const TSharedPtr<From>& a_Other) noexcept
{
	const auto Ptr = dynamic_cast<TSharedPtr<To>::APtr>(a_Other.Get());

	if (Ptr != nullptr)
	{
		return TSharedPtr<To>(a_Other, Ptr);
	}

	return nullptr;
}

/**
 * Dynamic pointer cast to downcast to a derived class
 * This requires RTTI
 *
 * This function invokes a move construction
 */
template <class To, class From>
FADE_NODISCARD TSharedPtr<To> DynamicPointerCast(TSharedPtr<From>&& a_Other) noexcept
{
	const To* Ptr = dynamic_cast<TSharedPtr<To>::APtr>(a_Other.Get());

	if (Ptr != nullptr)
	{
		return TSharedPtr<To>(Fade::Move(a_Other), Ptr);
	}

	return nullptr;
}

template <class ObjectType, class... Types>
TSharedPtr<ObjectType> MakeShared(Types&&... a_Args)
{
	const auto Ctrl = new TInlineReferenceController<ObjectType>(Fade::Forward<Types>(a_Args)...);
	TSharedPtr<ObjectType> Return;
	Return.InitFromPtrAndCtrl(__builtin_addressof(Ctrl->m_Storage), Ctrl);
	return Return;
}

template <typename ObjectType, EReferenceMode Mode>
class TWeakPtr
{

};
#else
template <typename T>
using TSharedPtr = std::shared_ptr<T>;

template <typename T>
using TWeakPtr = std::weak_ptr<T>;

template <typename Type>
bool IsValid(const TSharedPtr<Type>& a_SharedPtr)
{
	return a_SharedPtr.get() != nullptr;
}
#endif

}