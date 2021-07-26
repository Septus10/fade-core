#pragma once

#include <Core/CoreApi.hpp>
#include <Core/Reflection/Reflectable.hpp>
#include <Core/Reflection/ReflectableRegistry.hpp>
#include <Core/Reflection/Property.hpp>
#include <Core/Reflection/Struct.hpp>
#include <Core/Reflection/Class.hpp>
#include <Core/Reflection/Enumerator.hpp>

namespace Fade {

#define FADE_REFLECTABLE_BEGIN(reflectableName) \
static void register_##reflectableName##_() \
{	\
	Fade::CReflectableRegistry& Registry = Fade::CReflectableRegistry::Get();

#define FADE_REFLECTABLE_BEGIN_CLASS(reflectableName) \
	FADE_REFLECTABLE_BEGIN(reflectableName) \
	using AType = reflectableName; \
	Fade::CClass* Reflectable = new Fade::CClass(#reflectableName); 

#define FADE_REFLECTABLE_CLASS_DEFINE_MEMBER(type, className) \
	Reflectable->AddProperty(Fade::PropertyFactory<type>::MakeProperty(#className, offsetof(AType, className)));

#define FADE_REFLECTABLE_CLASS_SET_PARENT(parentName) \
	Reflectable->SetParent(static_cast<Fade::CStruct*>(Registry.GetReflectable(#parentName)));

#define FADE_REFLECTABLE_END(reflectableName) \
	Registry.RegisterReflectable(Reflectable); \
} namespace { \
struct refl_auto_register_##reflectableName##_ \
{ \
	refl_auto_register_##reflectableName##_() \
	{ \
		register_##reflectableName##_(); \
	} \
}; \
} \
static const refl_auto_register_##reflectableName##_ auto_register_##reflectableName##;

}
