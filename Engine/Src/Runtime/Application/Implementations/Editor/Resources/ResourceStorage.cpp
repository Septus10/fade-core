#include "ResourceStorage.hpp"
#include "Resource.hpp"

using namespace Fade;

CResource* CResourceStorage::GetResourceByHandle(AResourceHandle a_Handle)
{
	if (m_ResourcePointers.IsValidIndex(a_Handle))
	{
		return m_ResourcePointers[a_Handle];
	}

	return nullptr;
}

CResource* CResourceStorage::GetResourceByName(std::string a_Name)
{
	for (auto& Resource: m_ResourcePointers)
	{
		if (Resource->GetName().compare(a_Name) == 0)
		{
			return Resource;
		}
	}

	return nullptr;
}
