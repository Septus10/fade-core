#pragma once

#include <Core/CoreApi.hpp>
#include <Core/Definitions.hpp>

#include <string>

namespace Fade {

enum class EReflectableType : unsigned char
{
	Method,
	Property,
	Struct,
	Class,
	Enum
};

struct SHA256
{
	static usize Hash(const std::string& a_String);
};

class CReflectable
{
public:
	CReflectable(const char* a_Name);

	usize GetUniqueID() const
	{
		return m_UniqueID;
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	virtual EReflectableType GetType() const = 0;
	virtual const std::string& GetTypeString() const = 0;

protected:
	std::string		m_Name;
	usize			m_UniqueID;
};

}