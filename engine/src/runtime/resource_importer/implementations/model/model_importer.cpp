#include <model_importer.hpp>

#include <core/bootstrapping/bootstrapper.hpp>
#include <core/bootstrapping/module_info.hpp>
#include <core/service_locator/service_locator.hpp>

#include <definitions.hpp>
#include <resource.hpp>

#include <iostream>
#include <fstl/memory.hpp>
// sneaky include
#include "../texture/texture_importer.hpp"
#include <resource_importer_hub/resource_importer_hub.hpp>

using namespace fade;
using namespace resources;

FADE_BOOTSTRAP_MODULE(module_model_importer)
FADE_BOOTSTRAP_DEPENDENCIES(module_texture_importer)
FADE_BOOTSTRAP_ON_CONSTRUCT({
	std::cout << "Loaded model importer module\n";
	model_importer* imp = new model_importer();
	resource_importer_hub* imp_hub = get_service_locator().get_service<resource_importer_hub>();
	if (!imp_hub)
	{
		std::cout << "resource_importer_hub was not found by the service locator\n";
		return;
	}
	std::cout << "registering model importer\n";
	imp_hub->register_importer(std::make_unique<model_importer>(*imp));
})

model_importer::model_importer()
{	
	std::cout << "model_importer()\n";
	resource_importer_hub* imp_hub = get_service_locator().get_service<resource_importer_hub>();
	if (imp_hub)
	{
		tex_imp_ = dynamic_cast<texture_importer*>(imp_hub->get_importer("texture_importer"));
	}
	else
	{
		tex_imp_ = nullptr;
	}		
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
	std::vector<std::string> extensions_ = {
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

