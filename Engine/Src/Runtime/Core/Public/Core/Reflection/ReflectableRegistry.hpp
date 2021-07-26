#pragma once

#include <Core/Reflection/Reflectable.hpp>
#include <Core/Containers/Array.hpp>

namespace Fade {

class CReflectableRegistry
{
public:
	static CReflectableRegistry& Get();

	void RegisterReflectable(CReflectable* a_Reflectable);
	
	CReflectable* GetReflectable(const std::string& a_Name) const;
	const TArray<CReflectable*>& GetReflectables() const;

protected:
	CReflectableRegistry() {};

	TArray<CReflectable*> sm_Reflectables;
};

}