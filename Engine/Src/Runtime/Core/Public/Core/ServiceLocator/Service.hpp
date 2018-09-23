#pragma once

#include <Core/ServiceLocator/ServiceLocator.hpp>

#include <iostream>

namespace Fade
{
	
class FADE_CORE_API CService
{
public:
	CService()
	{
		GetServiceLocator().RegisterService(this);
	}

	virtual ~CService()
	{
		GetServiceLocator().UnregisterService(this);
	}
};

}
