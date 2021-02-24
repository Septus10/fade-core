#pragma once

#include <Core/Containers/Array.hpp>
#include <Core/Definitions.hpp>
#include <ECS/ECS.hpp>
#include <ECS/Component.hpp>

#include <Core/Utility/TemplateUtil.hpp>

#include <unordered_map>

namespace Fade { namespace ECS {

struct SComponentChunkHeader
{
	ComponentKey m_Components;
};

/*
 * A component chunk contains all components of a single archetype
 * that can be found in our game world
 */
 //struct SComponentChunk
 //{
 //	u8 m_Data[16384];
 //};
 //
 //class CComponentStore
 //{
 //public:
 //	template <class ComponentClass>
 //	ComponentClass* AddComponentToEntity(const class CEntity& a_Entity)
 //	{
 //		static_assert(TIsConvertible<ComponentClass, CComponentBase>::sm_Value, 
 //			"Unable to convert from ComponentClass to CComponentBase");
 //	}
 //
 //private:
 //	
 //
 //};

} }