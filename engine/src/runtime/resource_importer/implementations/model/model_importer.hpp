#pragma once

#include <resource_importer/resource_importer.hpp>

namespace fade { namespace resources {

class texture_importer;
class model_importer : public resource_importer
{
public:
	model_importer();
	~model_importer();
	
	std::unique_ptr<resource> import_resource(std::string file_path) override;
	bool imports_extension(std::string extension) override;
private:
	texture_importer* tex_imp_;
};

} }