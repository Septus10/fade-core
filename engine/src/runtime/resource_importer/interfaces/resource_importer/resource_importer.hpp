#pragma once

#include <resource_importer/resource_importer_api.hpp>

#include <vector>
#include <memory>

namespace fade { namespace resources {

class resource;
class FADE_API resource_importer abstract
{
public:
	virtual ~resource_importer() = default;
	virtual bool imports_extension(std::string extension) = 0;
	virtual std::unique_ptr<resource> import_resource(std::string file_path) = 0;

protected:
	std::vector<std::string> extensions_;
};

} }