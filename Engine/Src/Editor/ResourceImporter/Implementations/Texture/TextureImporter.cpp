#include <TextureImporter.hpp>

#include <Core/bootstrapping/bootstrapper.hpp>
#include <Core/bootstrapping/module_info.hpp>
#include <Core/Definitions.hpp>
#include <Core/ServiceLocator/ServiceLocator.hpp>


#include <iostream>

#include <ResourceImporterHub/Resource.hpp>
#include <ResourceImporterHub/ResourceImporterHub.hpp>

using namespace Fade;
using namespace Resources;

FADE_BOOTSTRAP_MODULE(CModuleTextureImporter)
FADE_BOOTSTRAP_ON_CONSTRUCT({
	//auto* imp_hub = GetServiceLocator().GetService<CResourceImporterHub>();
	//if (imp_hub)
	//{
	//	imp_hub->RegisterImporter<CTextureImporter>();
	//}
})

CTextureImporter::CTextureImporter()
{
}

CTextureImporter::~CTextureImporter()
{
}

Fade::TUniquePtr<CResource> CTextureImporter::ImportResource(std::string file_path)
{
	return Fade::MakeUnique<CResource>();
}

bool CTextureImporter::ImportsExtension(std::string extension)
{
	// extension list, I really need to figure out a cleaner way of doing this xd
	static std::vector<std::string> extensions_ = {
		".png",
		".jpg",
		".bmp",
		".tga"
	};

	// loop through extension list
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

