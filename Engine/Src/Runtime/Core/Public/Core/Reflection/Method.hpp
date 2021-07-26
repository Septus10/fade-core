#pragma once

#include <Core/Reflection/Reflectable.hpp>

namespace Fade {

class CMethod : public CReflectable
{
public:
	EReflectableType GetType() const override
	{
		return EReflectableType::Method;
	}

	const std::string& GetTypeString() const override
	{
		static std::string TypeStr("method");
		return TypeStr;
	}
};

}