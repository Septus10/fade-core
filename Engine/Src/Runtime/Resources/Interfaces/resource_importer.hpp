#pragma once

#include <resources_api.hpp>

#include <vector>
#include <memory>

namespace fade 
{

namespace resources
{

class resource;
class FADE_API resource_importer abstract
{
public:
	virtual bool imports_extension(std::string extension) = 0;
	virtual std::unique_ptr<resource> import_resource(std::string file_path) = 0;

};

std::vector<std::unique_ptr<resource_importer>> FADE_API get_importers();

}

}