#pragma once

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

template <class Type> 
inline constexpr typename TRemoveReference<Type>::TType&& Move(Type&& a_Arg) noexcept
{
	return (static_cast<typename TRemoveReference<Type>::TType&&>(a_Arg));
}

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

// Enable if default, no Type
template <bool Test, class Type = void>
struct TEnableIf
{};

// Enable if when true, type exists
template <class ValueType>
struct TEnableIf<true, ValueType>
{
	typedef ValueType TType;
};

template<class From, class To>
struct TIsConvertible : TPredicateBase<__is_convertible_to(From, To)>
{ };

}