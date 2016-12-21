#pragma once

#include <resource_manager.hpp>
#include <unordered_map>

namespace fade
{

namespace resources
{

class resource;
class resource_importer;
class resource_manager_impl : public resource_manager
{
public:
	resource_manager_impl();
	~resource_manager_impl();

	std::shared_ptr<resource> get_resource(std::string file_path) override;

private:
	std::unordered_map<std::string, std::shared_ptr<resource>> resource_map_;
	std::vector<std::unique_ptr<resource_importer>>	importers_;

};

}

}