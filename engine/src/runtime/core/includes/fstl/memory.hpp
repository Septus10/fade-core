#pragma once

#include <memory>

namespace fade
{
	namespace fstl
	{
		template <typename T>
		using unique_ptr = std::unique_ptr<T>;

		template <typename T>
		using shared_ptr = std::shared_ptr<T>;

		template <typename T>
		unique_ptr<T> make_unique()
		{
			return std::make_unique<T>();
		}

		template <typename T>
		shared_ptr<T> make_shared()
		{
			return std::make_shared<T>();
		}
	}
}