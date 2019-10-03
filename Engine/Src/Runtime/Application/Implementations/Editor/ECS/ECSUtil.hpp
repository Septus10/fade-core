#pragma once

#include <string>
#include <map>
#include <iostream>

#include <Core/Containers/Array.hpp>

namespace Fade { namespace ECS { namespace Util {

//https://gamedev.stackexchange.com/questions/17746/entity-component-systems-in-c-how-do-i-discover-types-and-construct-component/17759#17759
// Link used for creating this registry, my own additions have been made as well. 
//
// In the solution described by "alter igel" the application would throw an exception, caused by the order of initialization.
// The problem here was that the object map was not properly initialized before trying to register an object, 
// therefore I added a "singleton"-esque way of getting the object map before working with it to ensure it would be properly initialized before using it.

template <class BaseType>
struct Registry
{
protected:
	// Do not allow this struct to be instantiated
	Registry() = delete;

	// Object map alias
	using ObjectMap = std::map<std::string, BaseType*(*)(void)>;

	// Object map getter, also takes care of initialization
	static ObjectMap& GetObjectMap()
	{
		static ObjectMap sm_RegisteredObjects;
		return sm_RegisteredObjects;
	}

	// Object type creation func
	template <class ObjectType>
	static BaseType* Instantiate()
	{
		return new ObjectType;
	}

public:	
	template <class ObjectType>
	struct RegisterObject
	{
		RegisterObject(std::string a_Name)
		{
			GetObjectMap()[a_Name] = Instantiate<ObjectType>;
		}
	};

	static BaseType* CreateObject(const std::string& a_Name)
	{
		auto& ObjectMap = GetObjectMap();
		const auto& it = ObjectMap.find(a_Name);
		if (it == ObjectMap.end())
		{
			return nullptr;
		}
		else
		{
			return it->second();
		}
	}

	static TArray<std::string> GetRegisteredObjects()
	{
		TArray<std::string> ret;
		auto& ObjectMap = GetObjectMap();
		usize size = ObjectMap.size();
		ret.reserve(size);
		
		for (auto& it: ObjectMap)
		{
			ret.push_back(it.first);
		}
	
		return ret;
	}
	
	static usize GetNumRegisteredObjects()
	{
		return GetObjectMap().size();
	}
};


} } }


#define RegisterObject(base, type, name) \
namespace { \
	Fade::ECS::Util::Registry<base>::RegisterObject<type> register_object_##type(name); \
}