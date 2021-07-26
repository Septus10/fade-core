#pragma once

#include <Core/Reflection/Reflectable.hpp>
#include <Core/Reflection/Property.hpp>

#include <Core/Containers/LinkedList.hpp>

namespace Fade {

class CStruct : public CReflectable
{
public:
	CStruct(const char* a_Name)
		: CReflectable(a_Name)
		, m_Parent(nullptr)
	{ }

	EReflectableType GetType() const override
	{
		return EReflectableType::Struct;
	}

	void AddProperty(CProperty* a_Property)
	{
		m_Properties.Add(a_Property);
	}

	void SetParent(CStruct* a_ParentStruct)
	{
		m_Parent = a_ParentStruct;
		m_Properties.Append(m_Parent->m_Properties);
	}

	const std::string& GetTypeString() const override
	{
		static std::string TypeStr("struct");
		return TypeStr;
	}

	const TLinkedList<CProperty*>& GetProperties() const
	{
		return m_Properties;
	}

protected:
	TLinkedList<CProperty*> m_Properties;

	CStruct* m_Parent;

private:
	std::size_t GetNumProperties() const
	{
		return m_Properties.Size();
	}
};

}