#pragma once

#include <ResourceImporterHub/ResourceImporterHubApi.hpp>

#include <memory>
#include <Core/Containers/Array.hpp>
#include <Core/Pimpl.hpp>
#include <Core/ServiceLocator/ServiceLocator.hpp>

namespace Fade { namespace Resources {

class CResourceImporter;
class FADE_RESOURCEIMPORTERHUB_API CResourceImporterHub : public CService
{
public:
	CResourceImporterHub();
	~CResourceImporterHub();

	/**
	 * 
	 */
	bool ImportResource(std::string a_FilePath);
	
	/**
	 *
	 */
	template <typename T>
	void RegisterImporter()
	{
		static_assert(std::is_base_of<CResourceImporter, T>::value, "Trying to register importer using class that is not derived by CResourceImporter");
		m_Importers.push_back(new T());
	}

	/**
	 *
	 */
	template <typename ImporterClass>
	CResourceImporter* GetImporter()
	{
		for (auto& imp: m_Importers)
		{
			if (dynamic_cast<ImporterClass*>(imp.Get()))
			{
				return imp.Get();
			}
		}
		return nullptr;
	}

private:
	Fade::TArray<TUniquePtr<CResourceImporter>> m_Importers;
};

} }
