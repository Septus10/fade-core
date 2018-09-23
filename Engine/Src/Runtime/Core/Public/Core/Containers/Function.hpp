#pragma once

#include <functional>

namespace Fade
{
	namespace fstl
	{
		template <typename T>
		using Function = std::function<T>;
	}
}