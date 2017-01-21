#pragma once

#include <resource_importer/resource_importer.hpp>

namespace fade { namespace resources {

class texture_importer : public resource_importer
{
public:
	texture_importer();
	~texture_importer();

	std::unique_ptr<resource> import_resource(std::string file_path) override;
	bool imports_extension(std::string extension) override;

private:

};

} }
