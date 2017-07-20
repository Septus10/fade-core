#pragma once

#include <cstddef>
#include <cinttypes>

#include <memory>

namespace fade
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
 * Floating point types
 */
using f32 = float;
using f64 = double;

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

//===========================
// Fade types
//===========================
/**
 * Fade void pointer
 */
using vptr = void*;

}