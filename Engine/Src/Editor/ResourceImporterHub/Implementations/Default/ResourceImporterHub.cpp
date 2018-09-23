#include <ResourceImporterHub/ResourceImporterHub.hpp>
#include <ResourceImporterHub/resource.hpp>

#include <ResourceImporter/ResourceImporter.hpp>

using namespace Fade;
using namespace Resources;

CResourceImporterHub::CResourceImporterHub() {}
CResourceImporterHub::~CResourceImporterHub()
{
	for (auto& imp : m_Importers)
	{
		delete imp;
	}
}

bool CResourceImporterHub::ImportResource(std::string file_path)
{
	// get the extension of the file
	std::string ext = file_path.substr(file_path.find_last_of('.'));

	// look through the importers and see if we can import the file
	std::unique_ptr<CResource> res = nullptr;
	for (auto& imp: m_Importers)
	{
		if (imp->ImportsExtension(ext))
		{
			res = std::move(imp->ImportResource(file_path));
		}
	}
	
	// we're not able to import this resource
	if (res == nullptr)
	{
		return false;
	}

	// get the path of the resource
	std::string path = file_path.substr(0, file_path.find_last_of('\\'));	

	return true;
}