#pragma once

#include <core/core_api.hpp>
#include <core/definitions.hpp>
#include <core/fstl/vector.hpp>
#include <core/fstl/memory.hpp>
#include <iostream>

namespace fade
{

class service;
class FADE_API service_locator
{
public:
	service_locator() = default;
	~service_locator() = default;

	void register_service(service* service);
	void unregister_service(service* service);

	template<typename T>
	T* get_service()
	{
		for (usize i = 0; i < services_.size(); i++)
		{
			T* ptr = dynamic_cast<T*>(services_[i]);
			if (ptr)
			{
				return ptr;
			}
		}
		return nullptr;
	}

private:
	std::vector<service*> services_;
};

service_locator FADE_API &get_service_locator();

}
