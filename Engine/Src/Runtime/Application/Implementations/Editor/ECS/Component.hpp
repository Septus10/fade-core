#pragma once

#include <Core/Utility/TemplateUtil.hpp>
#include <ECS/ECS.hpp>

namespace Fade { namespace ECS {

inline constexpr u32 g_NumBytes = 10;

using ComponentKey = u8[g_NumBytes];
using ComponentID = u16;

static void UnpackSingle(ComponentKey& a_InOutKey, ComponentID a_ID)
{
	// High
	u8 Byte = (a_ID >> 8) & 0xFF;
	// Low
	u8 Value = a_ID & 0xFF;

	a_InOutKey[Byte] |= Value;
}

template <typename LastComponent>
static void UnpackIds(ComponentKey& a_InOutKey)
{
	UnpackSingle(a_InOutKey, Last);
}

template <typename FirstComponent, typename... Rest>
static void UnpackIds(ComponentKey& a_InOutKey)
{
	static_assert(TIsConvertible<FirstComponent, CComponentBase>::sm_Value,
		"Unable to convert from FirstComponent to CComponentBase");

	UnpackSingle(a_InOutKey, FirstComponent::GetID());
	UnpackIds<Rest...>(a_InOutKey);
}

template <typename... Components>
struct ComponentKeyFactory
{
	static bool MakeComponentKey(ComponentKey& a_InOutKey)
	{
		UnpackIds<Components...>(a_InOutKey);
		return true;
	}
};

/*
 * ID Implementation similarly to TUniqueID because we need some very specific behavior.
 * behavior that I don't know how to implement for the TUniqueID class at the moment.
 */
struct CComponentBase
{
public:
	Fade::ECS::ComponentID GetID() { return m_ComponentID; }

protected:
	static Fade::ECS::ComponentID GetNextID()
	{
		// High
		const u8 Byte = ((sm_TypeIdCounter >> 8) & 0xFF);
		// Low
		const u8 Value = (sm_TypeIdCounter & 0xFF);

		static constexpr u8 HighestBit = (1 << ((sizeof(u8) * 8) - 1));

		// If our value is already set to the highest available bit
		if (Value & HighestBit || sm_TypeIdCounter == 0)
		{
			// increment the high byte and reset the value to the first bit
			sm_TypeIdCounter = ((Byte + 1) << 8) + 1;			
		}
		else
		{
			sm_TypeIdCounter = (Byte << 8) | (Value << 1);
		}
		return sm_TypeIdCounter;
	}

	Fade::ECS::ComponentID m_ComponentID;

private:
	static Fade::ECS::ComponentID sm_TypeIdCounter;	
};

template <class DerivedComponentClass>
struct CComponent : public CComponentBase
{ 
public:
	CComponent() 
	{ 
		m_ComponentID = sm_TypeID;
	}

private:
	static Fade::ECS::ComponentID sm_TypeID;
};

template <class DerivedComponentClass>
ComponentID CComponent<DerivedComponentClass>::sm_TypeID = CComponentBase::GetNextID();

} }