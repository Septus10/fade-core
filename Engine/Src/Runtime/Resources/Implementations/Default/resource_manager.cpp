#include <resource_manager.hpp>
#include <resource_manager_impl.hpp>

#include <resource_importer.hpp>

using namespace fade;
using namespace resources;

std::unique_ptr<resource_manager> fade::resources::get_resource_manager()
{
	return std::make_unique<resource_manager_impl>();
}

fade::resources::resource_manager_impl::resource_manager_impl()
{
	importers_ = std::move(get_importers());	
}

fade::resources::resource_manager_impl::~resource_manager_impl()
{

}

std::shared_ptr<resource> resource_manager_impl::get_resource(std::string file_path)
{
	// grab the name of the resource (includes the extension)
	std::string name = file_path.substr(file_path.find_last_of('\\'));

	// try to find it in the resource map
	if (resource_map_.find(name) == resource_map_.end())
	{
		// if found, return it
		return resource_map_[name];
	}

	// else we'll import the file
	std::string ext = file_path.substr(file_path.find_last_of('.'));
	for (const std::unique_ptr<resource_importer>& imp: importers_)
	{
		if (imp->imports_extension(ext))
		{
			std::shared_ptr<resource> res = imp->import_resource(file_path);
			resource_map_.insert(std::pair<std::string, std::shared_ptr<resource>>(name, std::move(res)));
			return res;
		}
	}

	// if we get here that means we're unable to import the resource, throw an error
	// fade::log(verbosity::Error, "Unable to import resource with extension: %s", ext.c_str());
	return nullptr;
}