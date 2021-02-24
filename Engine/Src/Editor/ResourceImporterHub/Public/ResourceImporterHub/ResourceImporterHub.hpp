#pragma once

#include <ResourceImporterHub/ResourceImporterHubApi.hpp>

#include <memory>
#include <Core/Containers/String.hpp>
#include <Core/Containers/Array.hpp>
#include <Core/Pimpl.hpp>
#include <Core/ServiceLocator/ServiceLocator.hpp>

namespace Fade { namespace Resources {

class IResourceImporter;
class FADE_RESOURCEIMPORTERHUB_API CResourceImporterHub : public IService
{
public:
	CResourceImporterHub();
	~CResourceImporterHub();

	/**
	 * 
	 */
	bool ImportResource(Fade::String a_FilePath);
	
	/**
	 *
	 */
	void RegisterImporter(IResourceImporter* a_Importer);

	/**
	 *
	 */
	template <typename T>
	void RegisterImporter()
	{
		static_assert(std::is_base_of<IResourceImporter, T>::value, "Trying to register importer using class that is not derived by IResourceImporter");
		RegisterImporter(new T());
	}

private:
	FADE_MAKE_PIMPL;
};

} }
