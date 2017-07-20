#pragma once

#include <functional>

namespace fade
{
	namespace fstl
	{
		template <typename T>
		using function = std::function<T>;
	}
}