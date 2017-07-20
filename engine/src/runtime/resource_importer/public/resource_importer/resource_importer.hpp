#pragma once

#include <resource_importer/resource_importer_api.hpp>

#include <vector>
#include <memory>

namespace fade { namespace resources {

class resource;
class FADE_API resource_importer abstract
{
public:
	/**
	 * Mandatory virtual destructor
	 */
	virtual ~resource_importer() = default;

	/**
	 * Check whether this resource importer imports the files' extension
	 * @returns true if this importer can import the extension, false otherwise
	 */
	virtual bool imports_extension(std::string extension) = 0;

	/**
	 * Import the resource found at the given file path
	 * @param file_path File path to the resource
	 * @returns the resource if the import went well, otherwise it returns a nullptr
	 */
	virtual std::unique_ptr<resource> import_resource(std::string file_path) = 0;
};

} }