#pragma once

#include <ResourceImporterHub/IResourceImporterHub.hpp>

namespace Fade { namespace Resources { 

class CResourceImporterHub : public IResourceImporterHub
{
public:
	virtual ~CResourceImporterHub() override;

	virtual bool ImportResource(std::string a_FilePath);

	virtual void RegisterImporter(IResourceImporter* a_Importer);

private:
	
};

} }