#include <core/service_locator/service_locator.hpp>
#include <core/service_locator/service.hpp>

#include <algorithm>

namespace fade
{

void service_locator::register_service(service* service)
{
	services_.push_back(service);
}

void service_locator::unregister_service(service* service)
{
	for(int i = 0; i < services_.size(); i++)
	{
		if (services_[i] == service)
		{
			services_.erase(services_.begin() + i);
			return;
		}
	}
}

service_locator& get_service_locator()
{
	static service_locator g_service_locator;
	return g_service_locator;
}

}
