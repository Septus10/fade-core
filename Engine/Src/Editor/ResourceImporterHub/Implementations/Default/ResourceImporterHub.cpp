#include <ResourceImporterHub/ResourceImporterHub.hpp>
#include <ResourceImporterHub/resource.hpp>

#include <ResourceImporter/ResourceImporter.hpp>

using namespace Fade;
using namespace Resources;

class CResourceImporterHub::CImpl
{
public:
	TArray<TUniquePtr<IResourceImporter>> m_Importers;
};

CResourceImporterHub::CResourceImporterHub() 
	: FADE_INIT_PIMPL(CResourceImporterHub)
{ }

CResourceImporterHub::~CResourceImporterHub() 
{
	for (auto& imp : m_Impl->m_Importers)
	{
		delete imp.Release();
	}
}

bool CResourceImporterHub::ImportResource(std::string a_FilePath)
{
	// get the extension of the file
	std::string ext = a_FilePath.substr(a_FilePath.find_last_of('.'));

	// look through the importers and see if we can import the file
	TUniquePtr<CResource> res = nullptr;
	for (auto& imp: m_Impl->m_Importers)
	{
		if (imp->ImportsExtension(ext))
		{
			res = std::move(imp->ImportResource(a_FilePath));
		}
	}
	
	// we're not able to import this resource
	if (res == nullptr)
	{
		return false;
	}

	// get the path of the resource
	std::string path = a_FilePath.substr(0, a_FilePath.find_last_of('\\'));

	return true;
}

void CResourceImporterHub::RegisterImporter(IResourceImporter* a_Importer)
{
	m_Impl->m_Importers.Add(MakeUnique(a_Importer));
}