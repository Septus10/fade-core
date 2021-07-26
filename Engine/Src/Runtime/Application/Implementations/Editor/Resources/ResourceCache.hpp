#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Containers/String.hpp>
#include <Core/definitions.hpp>

#include <Core/Containers/SharedPointer.hpp>

namespace Fade {

using AResourceHandle = u32;

class CResource;

/**
 * Resource cache is our object that caches and handles these cached resources
 */
class CResourceCache
{
public:
	
	
private:
	TArray<TWeakPtr<CResource>> m_CachedResources;
};

}