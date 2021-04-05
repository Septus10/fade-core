#pragma once

#include <Core/CoreApi.hpp>
#include <Core/Definitions.hpp>
#include <Core/Containers/String.hpp>
#include <Core/Containers/Array.hpp>

/* Class and struct prefixes
* C = Regular classes
* S = Regular structs
* T = Templated classes or structs
* I = Interface
*/

/* Variable prefixes
* s_ = Static variable
* g_ = Global variable
* m_ = Member variable
* a_ = Function argument/parameter
*
* Prefixes can be combined, they are, however,
* to be combined using their proper word order.
* Examples:
* sm_ = Static member variable
* sg_ = Static global variable
*
* Local variables do not have a prefix
*/

#pragma warning (disable : 4251);

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

namespace Fade {

template <typename T>
void Swap(T& a_From, T& a_To)
{
	T Temp = a_To;
	a_To = a_From;
	a_From = Temp;
}

}