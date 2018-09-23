#include <Core/ServiceLocator/ServiceLocator.hpp>
#include <Core/ServiceLocator/Service.hpp>

#include <algorithm>

namespace Fade
{

void CServiceLocator::RegisterService(CService* service)
{
	m_Services.push_back(MakeUnique<CService>(service));
}

void CServiceLocator::UnregisterService(CService* service)
{
	for(int i = 0; i < m_Services.size(); i++)
	{
		if (m_Services[i] == service)
		{
			m_Services.erase(m_Services.begin() + i);
			return;
		}
	}
}

CServiceLocator& GetServiceLocator()
{
	static CServiceLocator g_ServiceLocator;
	return g_ServiceLocator;
}

}
