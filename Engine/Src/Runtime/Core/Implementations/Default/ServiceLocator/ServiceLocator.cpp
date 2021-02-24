#include <Core/ServiceLocator/ServiceLocator.hpp>

#include <algorithm>

namespace Fade
{

struct CServiceLocator
{
	TArray<TUniquePtr<IService>> m_Services;
};

static CServiceLocator g_ServiceLocator;

namespace ServiceLocator
{
	IService* RegisterService(TUniquePtr<IService>&& a_Service)
	{
		g_ServiceLocator.m_Services.Add(Fade::Move(a_Service));
		return g_ServiceLocator.m_Services.back().Get();
	}

	void UnregisterService(IService* a_Service)
	{
		
	}

	const TArray<TUniquePtr<IService>>& GetServices()
	{
		return g_ServiceLocator.m_Services;
	}
}

}
