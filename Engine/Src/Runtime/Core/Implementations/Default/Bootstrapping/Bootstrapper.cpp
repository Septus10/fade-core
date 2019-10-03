#pragma once

#include <core/bootstrapping/bootstrapper.hpp>
#include <core/definitions.hpp>

#include <vector>
#include <cassert>

namespace Fade { namespace Bootstrap { namespace Detail {

static std::vector<SModuleEvents>& ModuleList()
{
	static std::vector<SModuleEvents> list;
	return list;
}

callback SModuleEvents::execute_event = nullptr;

SModuleEvents::SModuleEvents(const u64 name_hash) :
	name_hash(name_hash),
	dependency_checked(false),
	dependency_count(0)
{
	events[event_construct] = nullptr;
	events[event_start] = nullptr;
	events[event_end] = nullptr;
	events[event_deconstruct] = nullptr;
}

OnConstruct::OnConstruct(SModuleEvents& module, callback callback)
{
	module.events[event_construct] = callback;
}

OnStart::OnStart(SModuleEvents& module, callback callback)
{
	module.events[event_start] = callback;
}

OnExecute::OnExecute(callback callback)
{
	SModuleEvents::execute_event = callback;
}

OnEnd::OnEnd(SModuleEvents& module, callback callback)
{
	module.events[event_end] = callback;
}

OnDeconstruct::OnDeconstruct(SModuleEvents& module, callback callback)
{
	module.events[event_deconstruct] = callback;
}

SModuleEvents& RegisterModule(u64 name_hash)
{
	ModuleList().push_back(SModuleEvents(name_hash));
	return ModuleList().back();
}

void InvokeModuleEvents(EEventType type)
{
	// Reset values
	for (auto& module : ModuleList())
	{
		module.dependency_count = module.dependencies.size();
		module.dependency_checked = false;
	}

	usize module_total = ModuleList().size();
	usize module_count = 0;

	// Run dependency check on every module until all modules are checked
	while (module_count < module_total)
	{
		for (auto module = ModuleList().begin(); module != ModuleList().end();)
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
				for (auto& m : ModuleList())
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
			assert((++module != ModuleList().end()) && "Dependency error detected!");
		}
	}
}
}

void InitializeModules()
{
	InvokeModuleEvents(Detail::event_construct);
	InvokeModuleEvents(Detail::event_start);
}

void DeinitializeModules()
{
	InvokeModuleEvents(Detail::event_end);
	InvokeModuleEvents(Detail::event_deconstruct);
}

} }