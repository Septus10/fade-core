#pragma once

#include <resource_importer_hub/resource_importer_hub.hpp>
#include <unordered_map>

namespace fade
{

namespace resources
{

class resource;
class resource_importer;
class resource_importer_hub_impl : public resource_importer_hub
{
public:
	resource_importer_hub_impl();
	~resource_importer_hub_impl() override;

	bool import_resource(std::string file_path) override;

	void register_importer(std::unique_ptr<resource_importer> importer) override;
	resource_importer* get_importer(std::string name) override;
private:
	std::vector<std::unique_ptr<resource_importer>>	importers_;

};

}

}