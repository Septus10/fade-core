#pragma once

#include <core/service_locator/service_locator.hpp>

#include <iostream>

namespace fade
{
	
class FADE_API service
{
public:
	service()
	{
		std::cout << "registering service with address: " << this << "\n";
		get_service_locator().register_service(this);
	}

	virtual ~service()
	{
		std::cout << "unregistering service with address: " << this << "\n";
		get_service_locator().unregister_service(this);
	}
};

}
