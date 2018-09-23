#pragma once

#include <Core/CoreApi.hpp>
#include <Core/Definitions.hpp>
#include <Core/Containers/UniquePointer.hpp>
#include <Core/Containers/Array.hpp>

namespace Fade
{

class CService;
class FADE_CORE_API CServiceLocator
{
public:
	CServiceLocator() = default;
	~CServiceLocator() = default;

	void RegisterService(CService* service);
	void UnregisterService(CService* service);

	template<typename T>
	T* GetService()
	{
		for (usize i = 0; i < m_Services.size(); i++)
		{
			T* ptr = dynamic_cast<T*>(m_Services[i]);
			if (ptr)
			{
				return ptr;
			}
		}
		return nullptr;
	}

private:
	TArray<TUniquePtr<CService>> m_Services;
};

CServiceLocator FADE_CORE_API &GetServiceLocator();

}
