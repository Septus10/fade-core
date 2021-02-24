#pragma once

#include <Core/CoreApi.hpp>
#include <Core/definitions.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <Core/Containers/Array.hpp>
#include <Core/Utility/TemplateUtil.hpp>
#include <type_traits>

namespace Fade
{

class FADE_CORE_API IService
{
public:
	IService() { }
	IService(const IService& a_Other) = delete;
	virtual ~IService() { }
};

namespace ServiceLocator
{
	/**
	 * Registers a new service to the service locator
	 *
	 * @param a_Service The service we want to add
	 * @return a pointer to the newly registered service
	 */
	FADE_CORE_API IService* RegisterService(TUniquePtr<IService>&& a_Service);
	
	template <class ServiceClass,
	typename = typename TEnableIf<std::is_base_of<IService, ServiceClass>::value>::TType>
	ServiceClass* RegisterService()
	{
		return dynamic_cast<ServiceClass*>(RegisterService(MakeUnique<ServiceClass>()));
	}

	/**
	 *  Unregisters a service from the service locator
	 *
	 * @param a_Service The service we want to remove
	 */
	FADE_CORE_API void UnregisterService(IService* a_Service);

	/**
	 * Gets an array of all the services
	 *
	 * @return The array of unique pointers which contains all the services
	 */
	FADE_CORE_API const TArray<TUniquePtr<IService>>& GetServices();

	/**
	 *
	 */
	template <class ServiceClass,
	typename = typename TEnableIf<std::is_base_of<IService, ServiceClass>::value>::TType>
	ServiceClass* GetService()
	{
		for (auto& service : GetServices())
		{
			if (auto* ptr = dynamic_cast<ServiceClass*>(service.Get()))
			{
				return ptr;
			}
		}

		// If we can't find the service, let's add the service
		return RegisterService<ServiceClass>();
	}
} 

}