#pragma once

#include <function.hpp>

namespace fade
{
	namespace fstl
	{
		template <typename T>
		using function = std::function<T>;
	}
}