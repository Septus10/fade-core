#pragma once

#include <Core/definitions.hpp>
#include <limits>

/**
 * This document is a custom implementation of template helpers that can be found in
 * <type_traits> 
 *
 * The reason for using this is mainly for having functionality in a uniform coding style
 * and additionally, it's great for learning purposes.
 */

namespace Fade {

template <typename Type>
struct TRemoveReference
{	// remove reference
	using TType = Type;
};

template <typename Type>
struct TRemoveReference<Type&>
{
	using TType = Type;
};

template <typename Type>
struct TRemoveReference<Type&&>
{
	using TType = Type;
};

template <typename Type>
struct TRemovePointer
{	// Remove pointer from type
	using TType = Type;
};

template <typename Type>
struct TRemovePointer<Type*>
{
	using TType = Type;
};

template <typename Type>
struct TRemoveExtent 
{
	using TType = Type;
};

template <typename Type, usize Size>
struct TRemoveExtent<Type[Size]>
{
	using TType = Type;
};

template <typename Type>
struct TRemoveExtent<Type[]>
{
	using TType = Type;
};

template <typename Type>
using TRemoveExtentType = typename TRemoveExtent<Type>::TType;

template <class ValueType, ValueType Value>
struct TStaticConstant
{
	// Static constant of our value type
	static constexpr ValueType sm_Value = Value;

	// New type for this struct where the templates are already filled in
	typedef TStaticConstant<ValueType, Value> TType;

	// Honestly, no idea what this does
	constexpr operator ValueType() const noexcept
	{
		return (Value);
	}

	// Neither do I know what this does
	constexpr ValueType operator()() const noexcept
	{
		return (Value);
	}
};


// TStaticConstant alias for booleans
template <bool Value>
using TBoolConstant = TStaticConstant<bool, Value>;

// TStaticConstant where type is boolean and value is true
typedef TStaticConstant<bool, true>		TTrueType;
// TStaticConstant where type is boolean and value is false
typedef TStaticConstant<bool, false>	TFalseType;

// Base class for type predicates
template <bool Value>
struct TPredicateBase : TStaticConstant<bool, Value>
{};


template <bool FirstValue, typename First, typename... Rest>
struct TDisjunctionInternal {
	using TType = First;
};

template <typename False, typename Next, typename... Rest>
struct TDisjunctionInternal<false, False, Next, Rest...> {
	using TType = typename TDisjunctionInternal<Next::sm_Value, Next, Rest...>::TType;
};

template <typename... Traits>
struct TDisjunction : TFalseType {};

template <typename First, typename... Rest>
struct TDisjunction<First, Rest...> : TDisjunctionInternal<First::sm_Value, First, Rest...>::TType
{ };

template <typename First, typename... Rest>
inline constexpr bool TDisjunctionValue = TDisjunction<First, Rest...>::sm_Value;

// Check if type is pointer
template <typename Type>
struct TIsPointer : TBoolConstant<false>
{ };

template <typename Type>
struct TIsPointer<Type*> : TBoolConstant<true>
{ };

// Check if type is void
template <typename Type>
struct TIsVoid : TBoolConstant<false>
{ };

template <>
struct TIsVoid<void> : TBoolConstant<true>
{ };

// Check if type is a reference (both r-value and regular)
template <typename Type>
struct TIsReference : TBoolConstant<false>
{ };

template <typename Type>
struct TIsReference<Type&> : TBoolConstant<true>
{ };

template <typename Type>
struct TIsReference<Type&&> : TBoolConstant<true>
{ };

template <typename Type>
struct TIsLvalueReference : TBoolConstant<false>
{ };

template <typename Type>
struct TIsLvalueReference<Type&> : TBoolConstant<true>
{ };

template <typename Type>
struct TIsRvalueReference : TBoolConstant<false>
{ };

template <typename Type>
struct TIsRvalueReference<Type&&> : TBoolConstant<true> 
{ };

template <typename Type>
inline constexpr bool TIsRvalueReferenceVal = TIsRvalueReference<Type>::sm_Value;

template <typename Type>
struct TIsArray : TBoolConstant<false>
{ };

template <typename Type>
struct TIsArray<Type[]> : TBoolConstant<true>
{ };

template <typename Type, usize Size>
struct TIsArray<Type[Size]> : TBoolConstant<true>
{ };

template <typename Type>
inline constexpr bool TIsArrayVal = TIsArray<Type>::sm_Value;

// Enable if default, no Type
template <bool Test, class Type = void>
struct TEnableIf
{};

// Enable if when true, type exists
template <class ValueType>
struct TEnableIf<true, ValueType>
{
	using TType = ValueType;
};

// Use type 1 if test is true
template <bool Test, class Type1, class Type2>
struct TConditional
{
	using TType = Type1;
};

// Use type 2 if test is false
template <class Type1, class Type2>
struct TConditional<false, Type1, Type2>
{
	using TType = Type2;
};

template<class From, class To>
struct TIsConvertible 
	: TPredicateBase<__is_convertible_to(const volatile From, const volatile To)>
{ };


template <class Base, class Derived>
struct TIsBaseOf
	: TPredicateBase<__is_base_of(Base, Derived)> 
{ };

#ifndef FADE_CPP_VERSION
#define FADE_CPP_VERSION 0
#endif

#ifndef FADE_CPP_20
#define FADE_CPP_20 20
#endif

#if FADE_CPP_VERSION == FADE_CPP_20
template <class Base, class Derived>
concept TIsDerivedFrom = 
	TIsBaseOf<Base, Derived>::sm_Value && 
	TIsConvertible<const volatile Derived*, const volatile Base*>::sm_Value;
#endif

template <bool FirstValue, class First, class... Rest>
struct TConjunctionBase
{
	using TType = First;
};

template <class True, class Next, class... Rest>
struct TConjunctionBase<true, True, Next, Rest...>
{
	using TType = typename TConjunctionBase<Next::sm_Value, Next, Rest...>::TType;
};

/**
 * If we have no traits, we are a true type
 */
template <class... Traits>
struct TConjunction : TTrueType 
{ };

/**
 * If we have traits, unpack until we find one that's false
 */
template <class First, class... Rest>
struct TConjunction<First, Rest...>
	: TConjunctionBase<First::sm_Value, First, Rest...>::TType
{ };

template <class... Traits>
inline constexpr bool TConjunctionVal = TConjunction<Traits...>::sm_Value;

template <class Type>
inline constexpr Type&& Forward(TRemoveReference<Type>& a_Arg) noexcept
{	// forward an lvalue as either an lvalue or an rvalue
	return (static_cast<Type&&>(a_Arg));
}

template <class Type>
inline constexpr Type&& Forward(TRemoveReference<Type>&& a_Arg) noexcept
{	// forward an rvalue as an rvalue
	static_assert(!is_lvalue_reference_v<Type>, "bad forward call");
	return (static_cast<Type&&>(a_Arg));
}

template <class Type>
inline constexpr typename TRemoveReference<Type>::TType&& Move(Type&& a_Arg) noexcept
{	// Move function
	return (static_cast<typename TRemoveReference<Type>::TType&&>(a_Arg));
}

#if 0

// Increment type bitshift
class IncrementBitshift
{
public:
	template <u32 PreviousID>
	static inline constexpr u32 IncrementID()
	{
		static_assert(PreviousID & (1 << sizeof(u32)), "Unable to increment ID using bit shifts, previous ID is already at max");
		return PreviousID << 1;
	}
};

#ifdef max
#undef max
#endif

// Increment type addition
class IncrementAddition
{
public:
	template <u32 PreviousID>
	static constexpr u32 IncrementID()
	{
		static_assert(PreviousID & std::numeric_limits<u32>::max(), "Unable to increment ID using addition, previous ID is already at max");
		return PreviousID + 1;
	}
};

template <class BaseClass, class IncrementClass = IncrementAddition>
class TUniqueIDBase
{
private:
	// Unique ID generator for classes
	static constexpr u32 sm_IdCounter = 1;

protected:
	static constexpr u32 GetNextID()
	{	
		return IncrementClass::IncrementID<sm_IdCounter++>();
	}
};

//template <class BaseClass>
//u32 TUniqueIDBase<BaseClass>::sg_IdCounter;

template <class BaseClass, class IDClass, typename IncrementNamespace = IncrementBitshift>
class TUniqueID : public TUniqueIDBase<BaseClass, IncrementNamespace>
{
public:
	static constexpr u32 GetID() { return sm_ID; }

private:
	static u32 sm_ID;
};

template <class BaseClass, class IDClass, typename IncrementNamespace>
u32 TUniqueID<BaseClass, IDClass, IncrementNamespace>::sm_ID = TUniqueIDBase<BaseClass, IncrementNamespace>::GetNextID();

#endif

}