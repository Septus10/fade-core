#include <model_importer.hpp>

#include <core/bootstrapping/bootstrapper.hpp>
#include <core/bootstrapping/module_info.hpp>

#include <definitions.hpp>
#include <resource.hpp>

#include <iostream>

using namespace fade;
using namespace resources;

FADE_MODULE_DEFINE(model_importer)
FADE_BOOTSTRAP_MODULE(module_model_importer)
FADE_BOOTSTRAP_ON_CONSTRUCT({
	std::cout << "Constructing model importer\n";
})

model_importer::model_importer()
{	
	tex_imp_ = nullptr;
	extensions_ = {
		".fbx",
		".obj"
	};
}

model_importer::~model_importer()
{
}

std::unique_ptr<resource> model_importer::import_resource(std::string file_path)
{
	return nullptr;
}

bool model_importer::imports_extension(std::string extension)
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

