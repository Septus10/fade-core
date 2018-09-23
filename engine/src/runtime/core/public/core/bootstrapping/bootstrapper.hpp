#pragma once

#include <core/definitions.hpp>
#include <core/bootstrapping/module_info.hpp>

#include <array>
#include <unordered_set>


namespace Fade { namespace Bootstrap { namespace Detail {
		
using callback = void(*)();

enum EEventType
{
	event_construct,
	event_start,
	event_end,
	event_deconstruct,

	event_total
};

struct SModuleEvents
{
	const u64 name_hash;

	bool dependency_checked;
	Fade::usize dependency_count;
	std::unordered_set<u64> dependencies;

	std::array<callback, event_total> events;
	static callback execute_event;

	explicit SModuleEvents(const u64 name_hash);
};

struct FADE_CORE_API OnConstruct
{
	explicit OnConstruct(SModuleEvents& module, callback callback);
};

struct FADE_CORE_API OnStart
{
	explicit OnStart(SModuleEvents& module, callback callback);
};

struct FADE_CORE_API OnExecute
{
	explicit OnExecute(callback callback);
};

struct FADE_CORE_API OnEnd
{
	explicit OnEnd(SModuleEvents& module, callback callback);
};

struct FADE_CORE_API OnDeconstruct
{
	explicit OnDeconstruct(SModuleEvents& module, callback callback);
};

template <typename T>
SModuleEvents& RegisterModule();
FADE_CORE_API SModuleEvents& RegisterModule(u64 name_hash);

template<typename... Args>
struct RegisterDependencies
{
	static bool Impl(SModuleEvents& module);
};

template<typename T>
struct RegisterDependencies<T>
{
	static bool Impl(SModuleEvents& module);
};

template<typename T, typename... Args>
struct RegisterDependencies<T, Args...>
{
	static bool Impl(SModuleEvents& module);
};

void InvokeModuleEvents(EEventType type);
	
}

void FADE_CORE_API InitializeModules();
void FADE_CORE_API DeinitializeModules();

} }

#define FADE_BOOTSTRAP_MODULE(module_class)	::Fade::Bootstrap::Detail::SModuleEvents& bootstrapModule = ::Fade::Bootstrap::Detail::RegisterModule<module_class>();

#define FADE_BOOTSTRAP_DEPENDENCIES(...) bool bootstrapDependencies = ::Fade::Bootstrap::Detail::RegisterDependencies<__VA_ARGS__>::Impl(bootstrapModule);

#define FADE_BOOTSTRAP_ON_CONSTRUCT(body) Fade::Bootstrap::Detail::OnConstruct construct(bootstrapModule, []()body);

#define FADE_BOOTSTRAP_ON_START(body) Fade::Bootstrap::Detail::OnStart start(bootstrapModule, []()body);

#define FADE_BOOTSTRAP_ON_END(body) Fade::Bootstrap::Detail::OnEnd end(bootstrapModule, []()body);

#define FADE_BOOTSTRAP_ON_DECONSTRUCT(body) Fade::Bootstrap::Detail::OnDeconstruct deconstruct(bootstrapModule, []()body);

#include "bootstrapper.inl"