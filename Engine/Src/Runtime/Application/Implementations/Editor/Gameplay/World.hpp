#pragma once

#include <ECS/ComponentStore.hpp>
#include <ECS/SystemManager.hpp>

namespace Fade {

class CWorld
{
public:
	void Tick(float a_DeltaTime);

private:
	//ECS::CSystemManager			m_SystemManager;
	//ECS::CComponentStore		m_ComponentStore;
};

}