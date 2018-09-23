#pragma once

namespace Fade { namespace Bootstrap { namespace Detail {
	
	template <typename T>
	SModuleEvents& RegisterModule()
	{
		return RegisterModule(T::name_hash);
	}

	template <typename ... Args>
	bool RegisterDependencies<Args...>::Impl(SModuleEvents&)
	{
		return false;
	}

	template <typename T>
	bool RegisterDependencies<T>::Impl(SModuleEvents& a_Module)
	{
		a_Module.dependencies.insert(T::name_hash);
		return true;
	}

	template <typename T, typename... Args>
	bool RegisterDependencies<T, Args...>::Impl(SModuleEvents& a_Module)
	{
		a_Module.dependencies.insert(T::name_hash);
		return RegisterDependencies<Args...>::Impl(a_Module);
	}

} } }