#include <resource_importer_hub/resource_importer_hub.hpp>
#include <resource_importer_hub_impl.hpp>

#include <resource_importer/resource_importer.hpp>
#include <resource.hpp>

using namespace fade;
using namespace resources;

std::unique_ptr<resource_importer_hub> fade::resources::get_resource_importer_hub()
{
	return std::make_unique<resource_importer_hub_impl>();
}

fade::resources::resource_importer_hub_impl::resource_importer_hub_impl()
{
}

fade::resources::resource_importer_hub_impl::~resource_importer_hub_impl()
{

}

bool resource_importer_hub_impl::import_resource(std::string file_path)
{
	// get the extension of the file
	std::string ext = file_path.substr(file_path.find_last_of('.'));

	// look through the importers and see if we can import the file
	std::unique_ptr<resource> res = nullptr;
	for (auto& imp: importers_)
	{
		if (imp->imports_extension(ext))
		{
			res = std::move(imp->import_resource(file_path));
		}
	}
	
	// we're not able to import this resource
	if (res == nullptr)
	{
		return false;
	}

	// get the path of the resource
	std::string path = file_path.substr(0, file_path.find_last_of('\\'));	



	return true;
	//// fade::log(verbosity::Error, "Unable to import resource with extension: %s", ext.c_str());
	//return nullptr;
}

void resource_importer_hub_impl::register_importer(std::unique_ptr<resource_importer> importer)
{
	importers_.push_back(move(importer));
}

resource_importer* resource_importer_hub_impl::get_importer(std::string name)
{
	for (auto& imp: importers_)
	{
		std::string str(typeid(imp).name());
		if (str.find(name) != std::string::npos)
		{
			return imp.get();
		}
	}
	return nullptr;
}
