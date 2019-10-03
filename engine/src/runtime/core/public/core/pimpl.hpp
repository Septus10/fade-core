#pragma once

#include <memory>
#include <Core/CoreApi.hpp>
#include <Core/Containers/UniquePointer.hpp>

#define FADE_MAKE_PIMPL                 \
private:								\
    class CImpl;                        \
    __pragma(warning(push))             \
    __pragma(warning(disable : 4251))   \
    TPimpl<CImpl> m_Impl;               \
    __pragma(warning(pop))

#define FADE_INIT_PIMPL(x) \
    m_Impl(MakeUnique<x::CImpl>())

/* Brief:
*  Creates pimpl object using a unique_ptr
*/
template <typename T>
class TPimpl
{
private:
	Fade::TUniquePtr<T> m_Ptr;

public:
	template <typename... Args>
	TPimpl(Args&&... args) :
		m_Ptr(std::forward<Args>(args)...)
	{ }

	// copy ctor & copy assignment
	TPimpl(const TPimpl& rhs) = default;
	TPimpl& operator=(const TPimpl& rhs) = default;

	// move ctor & move assignment
	TPimpl(TPimpl&& rhs) = default;
	TPimpl& operator=(TPimpl&& rhs) = default;

	// pointer operator
	T& operator*() { return *m_Ptr; }
	const T& operator*() const { return *m_Ptr; }

	// arrow operator
	T* operator->() { return m_Ptr.Get(); }
	const T* operator->() const { return m_Ptr.Get(); }

	// get() function
	T* get() { return m_Ptr.Get(); }
	const T* get() const { return m_Ptr.Get(); }
};
