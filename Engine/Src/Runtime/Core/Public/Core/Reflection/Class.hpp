#pragma once

#include <Core/Reflection/Struct.hpp>
#include <Core/Reflection/Method.hpp>

namespace Fade {

class CClass : public CStruct
{
public:
	CClass(const char* a_Name)
		: CStruct(a_Name)
	{ }

	EReflectableType GetType() const override
	{
		return EReflectableType::Class;
	}
	
	const std::string& GetTypeString() const override
	{
		static std::string TypeStr("class");
		return TypeStr;
	}

//private:
	TArray<CMethod*> m_Methods;	
};

}