#pragma once

#include <core/definitions.hpp>
#include <core/bootstrapping/module_info.hpp>

#include <array>
#include <unordered_set>


namespace fade { namespace bootstrap {
	namespace detail
	{
		using callback = void(*)();

		enum event_type
		{
			event_construct,
			event_start,
			event_end,
			event_deconstruct,

			event_total
		};

		struct module_events
		{
			const u64 name_hash;

			bool dependency_checked;
			fade::usize dependency_count;
			std::unordered_set<u64> dependencies;

			std::array<callback, event_total> events;
			static callback execute_event;

			explicit module_events(const u64 name_hash);
		};

		struct FADE_API on_construct
		{
			explicit on_construct(module_events& module, callback callback);
		};

		struct FADE_API on_start
		{
			explicit on_start(module_events& module, callback callback);
		};

		struct FADE_API on_execute
		{
			explicit on_execute(callback callback);
		};

		struct FADE_API on_end
		{
			explicit on_end(module_events& module, callback callback);
		};

		struct FADE_API on_deconstruct
		{
			explicit on_deconstruct(module_events& module, callback callback);
		};

		template <typename T>
		module_events& register_module();
		FADE_API module_events& register_module(u64 name_hash);

		template<typename... Args>
		struct register_dependencies
		{
			static bool impl(module_events& module);
		};

		template<typename T>
		struct register_dependencies<T>
		{
			static bool impl(module_events& module);
		};

		template<typename T, typename... Args>
		struct register_dependencies<T, Args...>
		{
			static bool impl(module_events& module);
		};

		void invoke_module_events(event_type type);
	}

	void FADE_API initialize_modules();
	void FADE_API deinitialize_modules();

} }

#define FADE_BOOTSTRAP_MODULE(module_class)	::fade::bootstrap::detail::module_events& bootstrap_module = ::fade::bootstrap::detail::register_module<module_class>();

#define FADE_BOOTSTRAP_DEPENDENCIES(...) bool bootstrap_dependencies = ::fade::bootstrap::detail::register_dependencies<__VA_ARGS__>::impl(bootstrap_module);

#define FADE_BOOTSTRAP_ON_CONSTRUCT(body) fade::bootstrap::detail::on_construct construct(bootstrap_module, []()body);

#define FADE_BOOTSTRAP_ON_START(body) fade::bootstrap::detail::on_start start(bootstrap_module, []()body);

#define FADE_BOOTSTRAP_ON_END(body) fade::bootstrap::detail::on_end end(bootstrap_module, []()body);

#define FADE_BOOTSTRAP_ON_DECONSTRUCT(body) fade::bootstrap::detail::on_deconstruct deconstruct(bootstrap_module, []()body);

#include "bootstrapper.inl"