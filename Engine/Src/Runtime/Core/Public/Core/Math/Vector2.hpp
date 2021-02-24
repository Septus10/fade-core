#pragma once

#include <Core/Math/Vector.hpp>

namespace Fade { namespace Math {

#if 0

template <typename Type>
struct Vector<2, Type>
{
	union {
		struct { Type X, Y; };
		struct { Type R, G; };
		struct { Type S, T; };
	};

	//=========================================================================
	// Data access
	//=========================================================================
	// Gets the number of dimensions of this vector (much like GLM's length function)
	static usize Dimensions() { return 2; }
	
	Type& operator[] (usize a_Index)
	{
		assert(a_Index >= 0 && a_Index < Dimensions());
		return (&X)[a_Index];
	}

	Type const& operator[] (usize a_Index) const
	{
		assert(a_Index >= 0 && a_Index < Dimensions());
		return (&X)[a_Index];
	}

	//=========================================================================
	// Constructors
	//=========================================================================
	// Implicit constructors
	Vector() 
		: X(Type{})
		, Y(Type{})
	{ }

	Vector(Vector const& a_Other)
		: X(a_Other.X)
		, Y(a_Other.Y)
	{ }

	// Explicit constructors
	explicit Vector(Type a_Scalar)
		: X(a_Scalar)
		, Y(a_Scalar)
	{ }

	explicit Vector(Type a_X, Type a_Y)
		: X(a_X)
		, Y(a_Y)
	{ }

	// Conversion constructor
	template <typename A, typename B>
	Vector(A a_X, B a_Y)
		: X(static_cast<Type>(a_X))
		, Y(static_cast<Type>(a_Y))
	{ }

	// Copy constructor
	template <u32 OtherDimensions, typename OtherType>
	Vector(const Vector<OtherDimensions, OtherType> const& a_Other)
	{
		assert(OtherDimensions >= this->Dimensions());
		X = static_cast<Type>(a_Other[0]);
		Y = static_cast<Type>(a_Other[1]);
	}
	//=========================================================================
};

typedef Vector<2, float> Vec2;

#endif

// struct Vec2
// {
// 	//=========================================================================
// 	// Members
// 	//=========================================================================
// 	// X-axis value
// 	float X;
// 	// Y-axis value
// 	float Y;
// 
// 	//=========================================================================
// 	// Statics
// 	//=========================================================================
// 	static FADE_CORE_API const Vec2 ZeroVector;
// 
// 	static FADE_CORE_API const Vec2 UnitVector;
// 
// 	//=========================================================================
// 	// Constructors
// 	//=========================================================================
// 	Vec2();
// 
// 	Vec2(float a_X, float a_Y);
// 
// 	//=========================================================================
// 	// Operators
// 	//=========================================================================
// 		// Arithmetic operators
// 	Vec2 operator+(const Vec2& a_Rhs) const;
// 	Vec2 operator+(const float a_Magnitude) const;
// 	Vec2 operator-(const Vec2& a_Rhs) const;
// 	Vec2 operator-(const float a_Magnitude) const;
// 	Vec2 operator*(const Vec2& a_Rhs) const;
// 	Vec2 operator*(const float a_Magnitude) const;
// 	Vec2 operator/(const Vec2& a_Rhs) const;
// 	Vec2 operator/(const float a_Magnitude) const;
// 		// Assignment operators
// 	Vec2 operator+=(const Vec2& a_Rhs);
// 	Vec2 operator+=(const float a_Magnitude);
// 	Vec2 operator-=(const Vec2& a_Rhs);
// 	Vec2 operator-=(const float a_Magnitude);
// 	Vec2 operator*=(const Vec2& a_Rhs);
// 	Vec2 operator*=(const float a_Magnitude);
// 	Vec2 operator/=(const Vec2& a_Rhs);
// 	Vec2 operator/=(const float a_Magnitude);
// 		// Relational operators
// 	bool operator==(const Vec2& a_Rhs) const;
// 	bool operator!=(const Vec2& a_Rhs) const;
// 	bool operator> (const Vec2& a_Rhs) const;
// 	bool operator< (const Vec2& a_Rhs) const;
// 	bool operator>=(const Vec2& a_Rhs) const;
// 	bool operator<=(const Vec2& a_Rhs) const;
// 
// 	//=========================================================================
// 	// Functionality
// 	//=========================================================================
// 	/**
// 	 * Get the length of this vector
// 	 *
// 	 * @return The length of this vector
// 	 */
// 	float Size() const;
// 
// 	/**
// 	 * Get the squared length of this vector
// 	 *
// 	 * @return The squared length of this vector
// 	 */
// 	float SizeSquared() const;
// 
// 	/**
// 	 * Checks whether the vector is nearly zero
// 	 * 
// 	 * @param a_Tolerance Error tolerance
// 	 * @return true if the vector is (nearly) zero, false otherwise
// 	 */
// 	bool IsNearlyZero(float a_Tolerance = 0.001f) const;
// 
// 	/**
// 	 * Checks whether all members of this vector are zero
// 	 *
// 	 * @return true if the vector is exactly zero, false otherwise
// 	 */
// 	bool IsZero() const;
// 
// 	/**
// 	 * Normalizes the vector
// 	 */
// 	void Normalize();
// };
// 
// //=============================================================================
// Vec2::Vec2() 
// 	: X(0.f)
// 	, Y(0.f)
// { }
// //=============================================================================
// Vec2::Vec2(float a_X, float a_Y) 
// 	: X(a_X)
// 	, Y(a_Y)
// { }
// //=============================================================================
// Vec2 Vec2::operator+(const Vec2& a_Rhs) const
// {
// 	return Vec2(X + a_Rhs.X, Y + a_Rhs.Y);
// }
// //=============================================================================
// Vec2 Vec2::operator+(const float a_Magnitude) const
// {
// 	return Vec2(X + a_Magnitude, Y + a_Magnitude);
// }
// //=============================================================================
// Vec2 Vec2::operator-(const Vec2& a_Rhs) const
// {
// 	return Vec2(X - a_Rhs.X, Y - a_Rhs.Y);
// }
// //=============================================================================
// Vec2 Vec2::operator-(const float a_Magnitude) const
// {
// 	return Vec2(X - a_Magnitude, Y - a_Magnitude);
// }
// //=============================================================================
// Vec2 Vec2::operator*(const Vec2& a_Rhs) const
// {
// 	return Vec2(X * a_Rhs.X, Y * a_Rhs.Y);
// }
// //=============================================================================
// Vec2 Vec2::operator*(const float a_Magnitude) const
// {
// 	return Vec2(X * a_Magnitude, Y * a_Magnitude);
// }
// //=============================================================================
// Vec2 Vec2::operator/(const Vec2& a_Rhs) const
// {
// 	return Vec2(X / a_Rhs.X, Y / a_Rhs.Y);
// }
// //=============================================================================
// Vec2 Vec2::operator/(const float a_Magnitude) const
// {
// 	return Vec2(X / a_Magnitude, Y / a_Magnitude);
// }
// //=============================================================================
// // Assignment operators
// //=============================================================================
// Vec2 Vec2::operator+=(const Vec2& a_Rhs)
// {
// 	X += a_Rhs.X;
// 	Y += a_Rhs.Y;
// 	return *this;
// }
// //=============================================================================
// Vec2 Vec2::operator+=(const float a_Magnitude)
// {
// 	X += a_Magnitude;
// 	Y += a_Magnitude;
// 	return *this;
// }
// //=============================================================================
// Vec2 Vec2::operator-=(const Vec2& a_Rhs)
// {
// 	X -= a_Rhs.X;
// 	Y -= a_Rhs.Y;
// 	return *this;
// }
// //=============================================================================
// Vec2 Vec2::operator-=(const float a_Magnitude)
// {
// 	X -= a_Magnitude;
// 	Y -= a_Magnitude;
// 	return *this;
// }
// //=============================================================================
// Vec2 Vec2::operator*=(const Vec2& a_Rhs)
// {
// 	X *= a_Rhs.X;
// 	Y *= a_Rhs.Y;
// 	return *this;
// }
// //=============================================================================
// Vec2 Vec2::operator*=(const float a_Magnitude)
// {
// 	X *= a_Magnitude;
// 	Y *= a_Magnitude;
// 	return *this;
// }
// //=============================================================================
// Vec2 Vec2::operator/=(const Vec2& a_Rhs)
// {
// 	X /= a_Rhs.X;
// 	Y /= a_Rhs.Y;
// 	return *this;
// }
// //=============================================================================
// Vec2 Vec2::operator/=(const float a_Magnitude)
// {
// 	X /= a_Magnitude;
// 	Y /= a_Magnitude;
// 	return *this;
// }
// //=============================================================================
// // Relational operators
// //=============================================================================
// bool Vec2::operator==(const Vec2& a_Rhs) const
// {
// 	return X == a_Rhs.X && Y == a_Rhs.Y;
// }
// //=============================================================================
// bool Vec2::operator!=(const Vec2& a_Rhs) const
// {
// 	return X != a_Rhs.X || Y != a_Rhs.Y;
// }
// //=============================================================================
// bool Vec2::operator> (const Vec2& a_Rhs) const
// {
// 	return SizeSquared() > a_Rhs.SizeSquared();
// }
// //=============================================================================
// bool Vec2::operator< (const Vec2& a_Rhs) const
// {
// 	return SizeSquared() < a_Rhs.SizeSquared();
// }
// //=============================================================================
// bool Vec2::operator>=(const Vec2& a_Rhs) const
// {
// 	return SizeSquared() >= a_Rhs.SizeSquared();
// }
// //=============================================================================
// bool Vec2::operator<=(const Vec2& a_Rhs) const
// {
// 	return SizeSquared() <= a_Rhs.SizeSquared();
// }
// //=============================================================================
// float Vec2::Size() const
// {
// 	return sqrtf(X*X + Y*Y);
// }
// //=============================================================================
// float Vec2::SizeSquared() const
// {
// 	return X*X + Y*Y;
// }
// //=============================================================================
// bool Vec2::IsNearlyZero(float a_Tolerance) const
// {
// 	return true;
// }
// //=============================================================================
// bool Vec2::IsZero() const
// {
// 	return X == 0 && Y == 0;
// }
// //=============================================================================
// void Vec2::Normalize()
// {
// 	//TODO: Implement
// }
// //=============================================================================

} }