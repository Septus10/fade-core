#pragma once

#include <functional>
#include <core/CoreApi.hpp>

namespace Fade
{

#ifdef NEW

class function
{
private:
	std::function<void()> function_;

public:
	function(std::function<void()> func) :
		function_(func)
	{ }

	void call()
	{
		function_();
	}
};

#endif
#ifdef OLD
#include <utility>
#include <tuple>
/**
* I'm still having tons of trouble wrapping my head around variadic templates and meta programming
* For that reason I'll leave this comment here explaining how the index generation works, always a nice refresher
* gen_sec<4> : gen_sec<3, 3, 4>
* gen_sec<3, 3, 4> : gen_sec<2, 2, 3, 4>
* gen_sec<2, 2, 3, 4> : gen_sec<1, 1, 2, 3, 4>
* gen_sec<1, 1, 2, 3, 4> : gen_sec<0, 0, 1, 2, 3, 4>
* gen_sec<0, 0, 1, 2, 3, 4> : index<0, 1, 2, 3, 4>
* index<0, 1, 2, 3, 4>
*/	
namespace helper
{
	template <size_t... Ts>
	struct index {};

	template <size_t N, size_t... Ts>
	struct gen_seq : gen_seq<N - 1, N - 1, Ts...> {};

	template <size_t... Ts>
	struct gen_seq<0, Ts...> : index<Ts...> {};
}

/**
* Abstract function base class, used to save multiple functions that have different parameters in one array
*/
class function_base abstract
{
public:
	/**
	* virtual dtor
	*/
	virtual ~function_base() = default;

	/**
	* use this function to invoke the saved function
	*/
	virtual void call() = 0;
};


/**
 * function template class, gives the user the ability to save functions and parameters for future use
 */
template <typename... Ts>
class function final : public function_base
{
private:
	std::function<void(Ts...)> function_;
	std::tuple<Ts...> arguments_;

public:
	/**
	 * function ctor
	 *
	 * @param func The function you want to save
	 * @param args The parameters you want to save
	 */
	function(std::function<void(Ts...)> func, Ts&&... args) :
		function_(func),
		arguments_(std::make_tuple(std::forward<Ts>(args)...))
	{ }

	/**
	 * after generating the index using the helper structs we now have the ability to actually invoke the saved function
	 */
	template <typename... Args, size_t... Is>
	void func(std::tuple<Args...>&& tup, helper::index<Is...>)
	{
		function_(std::get<Is>(std::move(tup))...);
	}

	/**
	 * use this function to invoke the saved function
	 */
	void call() override
	{
		func(std::move(arguments_), helper::gen_seq<sizeof...(Ts)>{});
	}
};
#endif

}