#pragma once

#include <core/bootstrapping/bootstrapper.hpp>

#include <definitions.hpp>

#include <vector>
#include <cassert>
#include <iostream>
#include <fstream>

namespace fade {
	namespace bootstrap {
		namespace detail {
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

				usize module_total = module_list().size();
				usize module_count = 0;

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

		std::vector<std::string> unpack_file(const char* filepath)
		{
			::std::ifstream file;
			file.open(filepath, std::ios::in | std::ios::binary);
			
			usize module_count;
			// read module count
			file.read(reinterpret_cast<c8*>(&module_count), sizeof(usize));

			// create entries list
			std::vector<std::string> entries;
			for (usize i = 0; i < module_count; i++)
			{
				usize length;
				// read module string lenght
				file.read(reinterpret_cast<c8*>(&length), sizeof(usize));
				// create char array with lenght of module string + 1 (for the added null terminator)
				char *module = new char[sizeof(c8) * length + 1];
				// read the actual string
				file.read(module, sizeof(c8) * length);
				module[length] = '\0';
				// push it into our entries list
				entries.push_back(std::string(module));
				// delete the remaining memory
				delete[] module;
			}
			return entries;
		}

		void pack_struct(std::vector<std::string> modules, const char* filepath)
		{
			// open file
			std::ofstream ofile;
			ofile.open(filepath, std::ios::out | std::ios::binary | std::ios::trunc);

			
			usize num = modules.size();
			// write num modules
			ofile.write(reinterpret_cast<c8*>(&num), sizeof(usize));

			// write modules
			for (usize i = 0; i < modules.size(); i++)
			{
				usize length = modules[i].length();
				ofile.write(reinterpret_cast<c8*>(&length) , sizeof(usize));
				ofile.write(modules[i].c_str(), modules[i].size());
			}
			
			ofile.flush();
		}

		// module loading includes (platform specific)
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else

#endif

	// module struct
		static std::vector<
#if defined(_WIN32) || defined(_WIN64)
			HMODULE
#else
			u64
#endif
		> g_mList;


		void load_module(const char* modulepath)
		{
#if defined(_WIN32) || defined(_WIN64)
			auto mod = LoadLibrary(modulepath);
#else

#endif
			g_mList.push_back(mod);
		}

		void load_modules()
		{
			// first we save (so that we can test
			pack_struct({ "./resource_importer_model.dll", "./resource_importer_texture.dll" }, "./modules.fmf");			

			// load config files
			std::vector<std::string> modules = unpack_file("./modules.fmf");
			for (usize i = 0; i < modules.size(); i++)
			{
				load_module(modules[i].c_str());
			}
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
			for (usize i = 0; i < g_mList.size(); i++)
			{
#if defined(_WIN32) || defined(_WIN64)
				if (!FreeLibrary(g_mList[i]))
				{
					std::cout << "Something went wrong unloading a module: " << GetLastError() << std::endl;
				}
#else

#endif
			}
		}
	}
}
