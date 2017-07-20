#include <texture_importer.hpp>

#include <core/bootstrapping/bootstrapper.hpp>
#include <core/bootstrapping/module_info.hpp>
#include <core/definitions.hpp>
#include <core/fstl/memory.hpp>

#include <resource_importer_hub/resource.hpp>

#include <iostream>

#include <resource_importer_hub/resource_importer_hub.hpp>
#include <core/service_locator/service_locator.hpp>
#include "resource_importer_hub_impl.hpp"

using namespace fade;
using namespace resources;

FADE_BOOTSTRAP_MODULE(module_texture_importer)
FADE_BOOTSTRAP_ON_CONSTRUCT({
	texture_importer* imp = new texture_importer();
	
	resource_importer_hub* imp_hub = get_service_locator().get_service<resource_importer_hub>();
	if (imp_hub)
	{
		imp_hub->register_importer(std::make_unique<texture_importer>(*imp));
	}
})

texture_importer::texture_importer()
{
}

texture_importer::~texture_importer()
{
}

std::unique_ptr<resource> texture_importer::import_resource(std::string file_path)
{
	return std::make_unique<resource>();
}

bool texture_importer::imports_extension(std::string extension)
{
	// loop through extension list
	std::vector<std::string> extensions_ = {
		".png",
		".jpg",
		".bmp",
		".tga"
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

