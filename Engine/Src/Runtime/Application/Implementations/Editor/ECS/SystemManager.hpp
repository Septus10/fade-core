#pragma once

#include <ECS/ComponentStore.hpp>

namespace Fade { namespace ECS {

class CSystemQueue
{
	
};

class CSystemManager
{
public:
	void Tick(float a_DeltaTime, const CComponentStore& a_ComponentStore)
	{}

private:
	CSystemQueue m_SystemQueue;
};

} }