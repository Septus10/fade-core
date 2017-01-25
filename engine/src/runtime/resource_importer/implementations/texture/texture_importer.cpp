#include <texture_importer.hpp>

#include <core/bootstrapping/bootstrapper.hpp>
#include <core/bootstrapping/module_info.hpp>

#include <definitions.hpp>
#include <resource.hpp>

#include <iostream>

using namespace fade;
using namespace resources;

FADE_MODULE_DEFINE(texture_importer)
FADE_BOOTSTRAP_MODULE(module_texture_importer)
FADE_BOOTSTRAP_ON_CONSTRUCT({
	std::cout << "Constructing texture importer\n";
})

texture_importer::texture_importer()
{
	extensions_ = {
		".png",
		".jpg",
		".bmp",
		".tga"
	};
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

