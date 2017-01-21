#pragma once

#include <core/bootstrapping/bootstrapper.hpp>

#include <definitions.hpp>

#include <vector>
#include <cassert>

namespace fade { namespace bootstrap {

	namespace detail
	{
		std::vector<module_events>& module_list()
		{
			static std::vector<module_events> list;
			return list;
		}

		callback module_events::execute_event = nullptr;

		module_events::module_events(const u64 name_hash) :
			name_hash(name_hash),
			dependency_checked(false),
			dependency_count(0)
		{
			events[event_construct] = nullptr;
			events[event_start] = nullptr;
			events[event_end] = nullptr;
			events[event_deconstruct] = nullptr;
		}

		on_construct::on_construct(module_events& module, callback callback)
		{
			module.events[event_construct] = callback;
		}

		on_start::on_start(module_events& module, callback callback)
		{
			module.events[event_start] = callback;
		}

		on_execute::on_execute(callback callback)
		{
			module_events::execute_event = callback;
		}

		on_end::on_end(module_events& module, callback callback)
		{
			module.events[event_end] = callback;
		}

		on_deconstruct::on_deconstruct(module_events& module, callback callback)
		{
			module.events[event_deconstruct] = callback;
		}
		
		module_events& register_module(u64 name_hash)
		{
			module_list().push_back(module_events(name_hash));
			return module_list().back();
		}

		void invoke_module_events(event_type type)
		{
			// Reset values
			for (auto& module : module_list())
			{
				module.dependency_count = module.dependencies.size();
				module.dependency_checked = false;
			}

			fade::usize module_total = module_list().size();
			fade::usize module_count = 0;

			// Run dependency check on every module until all modules are checked
			while (module_count < module_total)
			for (auto module = module_list().begin(); module != module_list().end();)
			{
				// Execute event if module has not been run yet and has no more dependencies;
				if (!module->dependency_checked && module->dependency_count == 0)
				{
					// Try to execute event
					if (module->events[type] != nullptr)
					{
						module->events[type]();
					}

					// Remove module from other modules dependency list
					for (auto m : module_list())
					{
						if (m.dependencies.find(module->name_hash) != m.dependencies.end())
						{
							m.dependency_count--;
						}
					}

					// Update count and restart dependency check
					module_count++;
					module->dependency_checked = true;
					break;
				}

				// Assert when we run into a circular dependency
				assert((++module != module_list().end()) && "Dependency error detected!");
			}
		}
	}

	void load_modules()
	{
		// load config files
	}

	void initialize_modules()
	{

		invoke_module_events(detail::event_construct);
		invoke_module_events(detail::event_start);
	}

	void deinitialize_modules()
	{
		invoke_module_events(detail::event_end);
		invoke_module_events(detail::event_deconstruct);
	}
} }
