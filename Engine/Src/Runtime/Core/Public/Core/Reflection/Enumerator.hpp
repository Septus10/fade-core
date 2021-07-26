#pragma once

#include <Core/Reflection/Reflectable.hpp>

#include <vector>

namespace Fade {

struct SEnumerator
{
	SEnumerator()
	{
		memset(m_Name, '\0', 128);
	}

	SEnumerator(const char a_Name[128], unsigned char a_Index)
		: m_Index(a_Index)
	{
		memset(m_Name, '\0', 128);
		memcpy(m_Name, a_Name, strlen(a_Name));
	}

	SEnumerator& operator=(const Fade::SEnumerator& a_Other)
	{
		memcpy(m_Name, a_Other.m_Name, strlen(a_Other.m_Name));
		m_Index = a_Other.m_Index;
		return *this;
	}

	char m_Name[128];
	unsigned char m_Index;
};

class CEnumerator : public CReflectable
{
public:
	CEnumerator(const char* a_Name)
		: CReflectable(a_Name)
	{
	}

	EReflectableType GetType() const override
	{
		return EReflectableType::Enum;
	}

	const std::string& GetTypeString() const override
	{
		static std::string TypeStr("enum");
		return TypeStr;
	}

	void SetEnumerators(std::vector<SEnumerator> a_Enumerators)
	{
		m_Enumerators = a_Enumerators;
	}

	const std::vector<SEnumerator> GetEnumerators() const 
	{
		return m_Enumerators;
	}

private:
	std::vector<SEnumerator> m_Enumerators;
};

}