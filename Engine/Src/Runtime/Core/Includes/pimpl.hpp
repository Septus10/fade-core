#pragma once

#include <memory>

#define FADE_MAKE_PIMPL                 \
public:									\
    class impl;                         \
private:								\
    __pragma(warning(push))             \
    __pragma(warning(disable : 4251))   \
    pimpl<impl> impl_;                  \
    __pragma(warning(pop))

/* Brief:
*  Creates pimpl object using a unique_ptr
*/
template <typename T>
class pimpl
{
private:
	std::unique_ptr<T> ptr_;

public:
	template <typename... Args>
	pimpl(Args&&... args) :
		ptr_(std::forward<Args>(args)...)
	{ }

	// copy ctor & copy assignment
	pimpl(const pimpl& rhs) = default;
	pimpl& operator=(const pimpl& rhs) = default;

	// move ctor & move assignment
	pimpl(pimpl&& rhs) = default;
	pimpl& operator=(pimpl&& rhs) = default;

	// pointer operator
	T& operator*() { return *ptr_; }
	const T& operator*() const { return *ptr_; }

	// arrow operator
	T* operator->() { return ptr_.get(); }
	const T* operator->() const { return ptr_.get(); }

	// get() function
	T* get() { return ptr_.get(); }
	const T* get() const { return ptr_.get(); }
};
