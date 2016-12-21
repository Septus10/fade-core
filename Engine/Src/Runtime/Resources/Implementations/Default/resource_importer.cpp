#include <resource_importer.hpp>

std::vector<std::unique_ptr<fade::resources::resource_importer>> fade::resources::get_importers()
{
	return std::vector<std::unique_ptr<fade::resources::resource_importer>>{};
}