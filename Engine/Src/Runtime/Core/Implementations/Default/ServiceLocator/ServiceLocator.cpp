#include <Core/ServiceLocator/ServiceLocator.hpp>

#include <algorithm>

namespace Fade
{

CService::CService()
{}
CService::~CService()
{}
CServiceLocator::CServiceLocator()
{}
CServiceLocator::~CServiceLocator()
{}

CService* CServiceLocator::RegisterService(TUniquePtr<CService>&& a_Service)
{
	//m_Services.push_back(std::move(a_Service));
	//return m_Services.back().Get();
	return nullptr;
}

void CServiceLocator::UnregisterService(CService* service)
{
	//for(int i = 0; i < m_Services.size(); i++)
	//{
	//	if (m_Services[i] == service)
	//	{
	//		m_Services.erase(m_Services.begin() + i);
	//		return;
	//	}
	//}
}

CServiceLocator& GetServiceLocator()
{
	static CServiceLocator g_ServiceLocator;
	return g_ServiceLocator;
}

}
