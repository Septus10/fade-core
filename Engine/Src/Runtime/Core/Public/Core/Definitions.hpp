#pragma once

#include <cstddef>
#include <cinttypes>

#ifndef FADE_HAS_NODISCARD
	#ifndef __has_cpp_attribute
	#define FADE_HAS_NODISCARD 0
	#elif __has_cpp_attribute(nodiscard) >= 201603L
	#define FADE_HAS_NODISCARD 1
	#else
	#define FADE_HAS_NODISCARD 0
	#endif
#endif

#if FADE_HAS_NODISCARD
	#define FADE_NODISCARD [[nodiscard]]
#else
	#define FADE_NODISCARD
#endif

namespace Fade
{

//===========================
// Regular types
//===========================

/**
 * Signed integer types
 */
using i8  = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

/**
 * Unsigned integer types
 */
using u8  = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

/**
 * Pointer types
 */
using iptr = std::intptr_t;
using uptr = std::uintptr_t;
using dptr = std::ptrdiff_t;

/**
 * Size type
 */
using usize = std::size_t;

/**
 * Char types
 */
using c8  = char;
using c16 = char16_t;
using c32 = char32_t;

/**
 * Unsigned char types
 */
using uc8 = unsigned char;

/**
 * Char types
 */
using wchar = wchar_t;

/**
 * String types
 */
using cstr = const char*;

//===========================
// Fade types
//===========================
/**
 * Fade void pointer
 */
using vptr = void*;
using nullptr_t = decltype(nullptr);

}