#pragma once

#include <resource_importer_hub/resource_importer_hub_api.hpp>
#include <resource.hpp>
#include <string>

#include <memory>

namespace fade {

namespace resources
{

class FADE_API resource_manager abstract
{
public:
	virtual ~resource_manager() = default;
	/**
	 * 
	 */
	virtual std::shared_ptr<resource> get_resource(std::string file_path) = 0;
private:


};

/**
 * Default factory
 */
std::unique_ptr<resource_manager> FADE_API get_resource_manager();

}

}