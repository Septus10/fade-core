#pragma once

#include <Core/Containers/Array.hpp> 

namespace Fade {

class CComponent;
class CEntity
{
public:
	//void AddComponent(CComponent* a_Component);
	//void RemoveComponent(CComponent* a_Component);

private:
	//STransform m_Transform;

protected:
	Fade::TArray<CComponent*>	m_Components;
	Fade::TArray<CEntity*>		m_Children;
};

}