#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/definitions.hpp>

namespace Fade {

using AResourceHandle = u32;

class CResource;
class CResourceStorage
{
public:
	CResource* GetResourceByHandle(AResourceHandle a_Handle);
	CResource* GetResourceByName(std::string a_Name);
	
private:
	TArray<CResource*> m_ResourcePointers;
};

}