#include <Core/Reflection/ReflectableRegistry.hpp>

namespace Fade {

CReflectableRegistry& CReflectableRegistry::Get()
{
	static CReflectableRegistry sm_Registry;
	return sm_Registry;
}

void CReflectableRegistry::RegisterReflectable(CReflectable* a_Reflectable)
{
	if (a_Reflectable != nullptr)
	{
		sm_Reflectables.push_back(a_Reflectable);
	}
}

CReflectable* CReflectableRegistry::GetReflectable(const std::string& a_Name) const
{
	std::size_t NameHash = SHA256::Hash(a_Name);
	for (auto& Reflectable : sm_Reflectables)
	{
		if (Reflectable->GetUniqueID() == NameHash)
		{
			return Reflectable;
		}
	}

	return nullptr;
}

const TArray<CReflectable*>& CReflectableRegistry::GetReflectables() const
{
	return sm_Reflectables;
}

}