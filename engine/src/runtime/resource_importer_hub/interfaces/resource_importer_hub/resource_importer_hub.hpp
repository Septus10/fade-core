#pragma once

#include <resource_importer_hub/resource_importer_hub_api.hpp>
#include <string>

#include <memory>
#include <core/service_locator/service.hpp>

namespace fade {

namespace resources
{

class resource_importer;
class FADE_API resource_importer_hub abstract : public service
{
public:
	virtual ~resource_importer_hub() = default;
	/**
	 * 
	 */
	virtual bool import_resource(std::string file_path) = 0;
	
	virtual void register_importer(std::unique_ptr<resource_importer> importer) = 0;
	virtual resource_importer* get_importer(std::string name) = 0;
};

/**
 * Default factory
 */
std::unique_ptr<resource_importer_hub> FADE_API get_resource_importer_hub();

}

}
