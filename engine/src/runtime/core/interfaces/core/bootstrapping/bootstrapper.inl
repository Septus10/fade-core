#pragma once

namespace fade { namespace bootstrap { namespace detail {
	
	template <typename T>
	module_events& register_module()
	{
		return register_module(T::name_hash);
	}

	template <typename ... Args>
	bool register_dependencies<Args...>::impl(module_events&)
	{
		return false;
	}

	template <typename T>
	bool register_dependencies<T>::impl(module_events& module)
	{
		module.dependencies.insert(T::name_hash);
		return true;
	}

	template <typename T, typename... Args>
	bool register_dependencies<T, Args...>::impl(module_events& module)
	{
		module.dependencies.insert(T::name_hash);
		return register_dependencies<Args...>::impl(module);
	}

} } }