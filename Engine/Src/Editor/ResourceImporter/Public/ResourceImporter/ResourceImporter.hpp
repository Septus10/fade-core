#pragma once

#include <ResourceImporter/ResourceImporterApi.hpp>

#include <vector>
#include <memory>

namespace Fade { namespace Resources {

class CResource;
class FADE_RESOURCEIMPORTER_API CResourceImporter
{
public:
	/**
	 * Mandatory virtual destructor
	 */
	virtual ~CResourceImporter() = default;

	/**
	 * Check whether this resource importer imports the files' extension
	 * @returns true if this importer can import the extension, false otherwise
	 */
	virtual bool ImportsExtension(std::string extension) = 0;

	/**
	 * Import the resource found at the given file path
	 * @param file_path File path to the resource
	 * @returns the resource if the import went well, otherwise it returns a nullptr
	 */
	virtual std::unique_ptr<CResource> ImportResource(std::string file_path) = 0;
};

} }