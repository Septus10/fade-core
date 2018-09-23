#include <Run/EngineLoop.hpp>
#include <core/timer.hpp>

#include <application/application.hpp>

#include <core/fstl/iostream.hpp>
#include <core/fstl/fstream.hpp>
#include <core/fstl/vector.hpp>
#include <core/fstl/string.hpp>

#include <Core/Bootstrapping/Bootstrapper.hpp>

namespace Fade {

CEngineLoop::CEngineLoop()
{
	m_ShouldRun = true;
    
    // set up application
	m_Application = GetApplication();
	
    // start engine timer
    m_Timer = std::make_unique<CTimer>();
    m_Timer->Start();
}

CEngineLoop::~CEngineLoop()
{
	
}

i32 CEngineLoop::PreInitialize()
{
	m_Application->PreInitialize();

	LoadModules();
	
	// initialize modules
	Bootstrap::InitializeModules();
	return 0;
}

i32 CEngineLoop::Initialize()
{
	m_Application->Initialize();
	return 0;
}

i32 CEngineLoop::PostInitialize()
{
	m_Application->PostInitialize();
	return 0;
}

i32 CEngineLoop::DeInitialize()
{
	m_Application->DeInitialize();

	Bootstrap::DeinitializeModules();

	UnloadModules();
	return 0;
}

void CEngineLoop::Tick()
{	
	if (m_Application->Tick(m_Timer->Elapsed()) == FADE_REQUEST_STOP)
	{
		m_ShouldRun = false;
	}
}

void CEngineLoop::ShouldStop(bool a_ShouldStop)
{
	m_ShouldRun = !a_ShouldStop;
}

bool CEngineLoop::ShouldStop() const
{
	return m_ShouldRun;
}

static fstl::Vector<fstl::String> unpack_file(const char* filepath)
{
	fstl::Ifstream file;
	file.open(filepath, std::ios::in | std::ios::binary);
	if(!file)
	{
		return fstl::Vector<fstl::String>();
	}

	usize module_count;
	// read module count
	file.read(reinterpret_cast<c8*>(&module_count), sizeof(usize));

	// create entries list
	fstl::Vector<fstl::String> entries;
	for (usize i = 0; i < module_count; i++)
	{
		usize length = 0;
		// read module string lenght
		file.read(reinterpret_cast<c8*>(&length), sizeof(usize));
		// create char array with lenght of module string + 1 (for the added null terminator)
		char *module = new char[sizeof(c8) * length + 1];
		// read the actual string
		file.read(module, sizeof(c8) * length);
		module[length] = '\0';
		// push it into our entries list
		entries.push_back(fstl::String(module));
		// delete the remaining memory
		delete[] module;
	}
	return entries;
}

static void pack_struct(fstl::Vector<fstl::String> modules, const char* filepath)
{
	// open file
	fstl::Ofstream ofile;
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

void CEngineLoop::LoadModules()
{
	pack_struct({ "ResourceImporterModel.dll", "ResourceImporterTexture.dll" }, "./modules.fmf");

	// load config files
	auto modules = unpack_file("./modules.fmf");
	for (usize i = 0; i < modules.size(); i++)
	{
		load_module(modules[i].c_str());
	}
}

void CEngineLoop::UnloadModules()
{
	for (usize i = 0; i < m_ModuleList.size(); i++)
	{
		if (!unload_module(m_ModuleList[i]))
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