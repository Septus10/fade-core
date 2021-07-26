#pragma once

#include <Core/Definitions.hpp>

#include <Core/Reflection/Reflection.hpp>
#include <Core/Reflection/Enumerator.hpp>

#include <Core/Containers/Array.hpp>

#include <Core/Utility/TemplateUtil.hpp>

namespace Fade {

enum class EPropertyType : unsigned char
{
	Integer,
	UnsignedInteger,
	Short,
	UnsignedShort,
	Byte,
	UnsignedByte,
	Boolean,
	Float,
	Double,
	String,
	WideString,
	Enum,
	Class,
	Struct,
	Array
};

class CProperty : public CReflectable
{
public:
	CProperty(const char* a_Name, usize a_Size, usize a_Offset, EPropertyType a_Type)
		: CReflectable(a_Name)
		, m_Size(a_Size)
		, m_Offset(a_Offset)
		, m_Type(a_Type)
	{
	}

	EReflectableType GetType() const override
	{
		return EReflectableType::Property;
	}

	const std::string& GetTypeString() const override
	{
		static std::string TypeStr("property");
		return TypeStr;
	}

	usize GetSize() const 
	{
		return m_Size;
	}

	usize GetOffset() const 
	{
		return m_Offset;
	}

private:
	usize			m_Size;
	usize			m_Offset;
	EPropertyType	m_Type;
};

class CEnumeratorProperty : public CProperty
{
public:
	explicit CEnumeratorProperty(const char* a_Name, usize a_Size, usize a_Offset, EPropertyType a_Type) noexcept
		: CProperty(a_Name, a_Size, a_Offset, a_Type)
		, m_Enumerator(nullptr)
	{
	}

	void __Internal_InitializeEnumPtr(const char* a_Name)
	{
		CReflectableRegistry& Registry = CReflectableRegistry::Get();
		CEnumerator* Enumerator = dynamic_cast<CEnumerator*>(Registry.GetReflectable(a_Name));
		if (Enumerator != nullptr)
		{
			m_Enumerator = Enumerator;
		}
	}

private:
	CEnumerator* m_Enumerator;
};

template <typename, typename = void>
struct PropertyFactory;

#define DEFINE_MEMBER_FN(type, enumType) \
template<> \
struct PropertyFactory<type, void> \
{ \
	static CProperty* MakeProperty(const char* a_Name, size_t a_MemberOffset) \
	{ \
	return new CProperty(a_Name, sizeof(type), a_MemberOffset, EPropertyType::##enumType); \
	} \
};

DEFINE_MEMBER_FN(float, Float)
DEFINE_MEMBER_FN(double, Double)
DEFINE_MEMBER_FN(int, Integer)
DEFINE_MEMBER_FN(unsigned int, UnsignedInteger)
DEFINE_MEMBER_FN(short, Short)
DEFINE_MEMBER_FN(unsigned short, UnsignedShort)
DEFINE_MEMBER_FN(char, Byte)
DEFINE_MEMBER_FN(unsigned char, UnsignedByte)
DEFINE_MEMBER_FN(bool, Boolean)

/**
 * enum specialization for member factory
 */
	template <typename T>
struct PropertyFactory<T, typename TEnableIf<std::is_enum_v<T>>::type>
{
	static CProperty* MakeProperty(const char* a_Name, size_t a_Offset)
	{
		// create enum property
		CEnumeratorProperty* Prop = new CEnumeratorProperty(a_Name, sizeof(T), a_Offset, EPropertyType::Enum);
		// TODO: we should defer the initialization of the enum pointer to a later point
		// where all reflected enums have been initialized
		Prop->__Internal_InitializeEnumPtr<T>();
		return Prop;
	}
};

/**
 * dynamic array specialization for member factory
 */
template <typename T>
struct PropertyFactory<TArray<T>, void>
{
	static CProperty* MakeProperty(const char* a_Name, size_t a_Offset)
	{
		return nullptr;
	}
};

}