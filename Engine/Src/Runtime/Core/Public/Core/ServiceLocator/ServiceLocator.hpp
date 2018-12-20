#pragma once

#include <Core/CoreApi.hpp>
#include <Core/definitions.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <Core/Containers/Array.hpp>

namespace Fade
{

class FADE_CORE_API CService
{
public:
	CService();
	CService(const CService& a_Other) = delete;
	virtual ~CService();
};

/*
 * Collection of all services that should be globally available
 */
class FADE_CORE_API CServiceLocator
{
public:
	CServiceLocator();
	~CServiceLocator();

	CServiceLocator(CServiceLocator&& a_Other) = delete;
	CServiceLocator(const CServiceLocator& a_Other) = delete;

	CServiceLocator& operator=(CServiceLocator&& a_Other) = delete;
	CServiceLocator& operator=(const CServiceLocator& a_Other) = delete;

	//template <
	//	class ServiceClass,
	//	typename = typename TEnableIf<std::is_base_of<CService, ServiceClass>::value>::TType
	//> ServiceClass* GetService()
	//{
	//	for (usize i = 0; i < m_Services.size(); i++)
	//	{
	//		ServiceClass* ptr = dynamic_cast<ServiceClass*>(m_Services[i].Get());
	//		if (ptr)
	//		{
	//			return ptr;
	//		}
	//	}
	//	
	//	// If we can't find the service, let's add the service
	//	return RegisterService(std::move(Fade::MakeUnique<ServiceClass>()));
	//}


private:
	TArray<TUniquePtr<CService>> m_Services;

	CService* RegisterService(TUniquePtr<CService>&& a_Service);
	void UnregisterService(CService* service);
};

CServiceLocator FADE_CORE_API &GetServiceLocator();

}
