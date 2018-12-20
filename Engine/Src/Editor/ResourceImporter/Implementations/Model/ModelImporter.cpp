#include <ModelImporter.hpp>

#include <Core/bootstrapping/bootstrapper.hpp>
#include <Core/bootstrapping/module_info.hpp>
#include <Core/ServiceLocator/ServiceLocator.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <Core/definitions.hpp>

#include <ResourceImporterHub/Resource.hpp>
#include <ResourceImporterHub/ResourceImporterHub.hpp>

#include <iostream>

using namespace Fade;
using namespace Resources;

FADE_BOOTSTRAP_MODULE(CModuleModelImporter)
FADE_BOOTSTRAP_DEPENDENCIES(CModuleModelImporter)
FADE_BOOTSTRAP_ON_CONSTRUCT({
	//auto* imp_hub = GetServiceLocator().GetService<CResourceImporterHub>();
    //if (!imp_hub)
    //{
    //    std::cout << "resource_importer_hub was not found by the service locator\n";
    //    return;
    //}
	//imp_hub->RegisterImporter<CModelImporter>();
})

CModelImporter::CModelImporter()
{	
	
}

CModelImporter::~CModelImporter()
{
}

Fade::TUniquePtr<CResource> CModelImporter::ImportResource(std::string file_path)
{
	return nullptr;
}

bool CModelImporter::ImportsExtension(std::string extension)
{
	// loop through extension list
	static std::vector<std::string> extensions_ = {
		".fbx",
		".obj"
	};

	for (usize i = 0; i < extensions_.size(); i++)
	{
		// if the extension exists, we return true
		if (extensions_[i].compare(extension) == 0)
		{
			return true;
		}
	}

	// else we return false
	return false;
}

