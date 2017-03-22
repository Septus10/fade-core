#include <run/engine_loop.hpp>
#include <core/timer.hpp>

#include <application.hpp>

#include <fstl/iostream.hpp>
#include <fstl/fstream.hpp>
#include <fstl/vector.hpp>
#include <fstl/string.hpp>

#include <core/bootstrapping/bootstrapper.hpp>
#include <winbase.h>

namespace fade {

engine_loop::engine_loop()
{
	run_ = true;
	app_ = get_application();
	timer_ = get_timer();
}

engine_loop::~engine_loop()
{
	
}

i32 engine_loop::pre_initialize()
{
	app_->pre_initialize();

	load_modules();
	
	// initialize modules
	bootstrap::initialize_modules();
	return 0;
}

i32 engine_loop::initialize()
{
	app_->initialize();
	return 0;
}

i32 engine_loop::post_initialize()
{
	timer_->start();
	app_->post_initialize();
	return 0;
}

i32 engine_loop::deinitialize()
{
	app_->deinitialize();

	bootstrap::deinitialize_modules();

	unload_modules();
	return 0;
}

void engine_loop::tick()
{	
	app_->tick(timer_->elapsed());
}

void engine_loop::should_stop(bool should_stop)
{
	run_ = should_stop;
}

bool engine_loop::should_stop() const
{
	return run_;
}

static fstl::vector<fstl::string> unpack_file(const char* filepath)
{
	fstl::ifstream file;
	file.open(filepath, std::ios::in | std::ios::binary);
	if(!file)
	{
		return fstl::vector<fstl::string>();
	}

	usize module_count;
	// read module count
	file.read(reinterpret_cast<c8*>(&module_count), sizeof(usize));

	// create entries list
	fstl::vector<fstl::string> entries;
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
		entries.push_back(fstl::string(module));
		// delete the remaining memory
		delete[] module;
	}
	return entries;
}

static void pack_struct(fstl::vector<fstl::string> modules, const char* filepath)
{
	// open file
	fstl::ofstream ofile;
	ofile.open(filepath, std::ios::out | std::ios::binary | std::ios::trunc);

	usize num = modules.size();
	// write num modules
	ofile.write(reinterpret_cast<c8*>(&num), sizeof(usize));

	// write modules
	for (usize i = 0; i < modules.size(); i++)
	{
		usize length = modules[i].length();
		ofile.write(reinterpret_cast<c8*>(&length), sizeof(usize));
		ofile.write(modules[i].c_str(), modules[i].size());
	}

	ofile.flush();
}

#ifdef FADE_PLATFORM_WINDOWS
#include <windows.h>
#include <winnt.h>
#elif FADE_PLATFORM_LINUX

#elif FADE_PLATFORM_MAC

#endif

static module_handle load_module(const char* modulepath)
{
#ifdef FADE_PLATFORM_WINDOWS
	return LoadLibrary(modulepath);
#elif FADE_PLATFORM_LINUX

#elif FADE_PLATFORM_MAC

#endif
}

static bool unload_module(module_handle module)
{
#ifdef FADE_PLATFORM_WINDOWS
	if (FreeLibrary(module) == 0)
	{
		return false;
	}
	return true;
#elif FADE_PLATFORM_LINUX
	return free_library_linux(module)
#elif FADE_PLATFORM_MAC
	return free_library_mac(module)
#endif
}

void engine_loop::load_modules()
{
	pack_struct({ "resource_importer_model.dll", "resource_importer_texture.dll" }, "./modules.fmf");

	// load config files
	auto modules = unpack_file("./modules.fmf");
	for (usize i = 0; i < modules.size(); i++)
	{
		load_module(modules[i].c_str());
	}
}

void engine_loop::unload_modules()
{
	for (usize i = 0; i < module_list_.size(); i++)
	{
		if (!unload_module(module_list_[i]))
		{
			std::cout << "Something went wrong unloading a module: " << 
#ifdef FADE_PLATFORM_WINDOWS
				GetLastError() 
#elif FADE_PLATFORM_LINUX
				get_last_error_linux()
#elif FADE_PLATFORM_MAC
				get_last_error_mac()
#endif
				<< "\n";
		}
	}
}

}