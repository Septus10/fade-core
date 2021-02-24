#pragma once

#include <Core/CoreApi.hpp>
#include <Core/Definitions.hpp>

#include <Core/Math/Math.hpp>

namespace Fade {

template <typename Type>
struct SRect
{
	union 
	{
		struct 
		{
			Type m_Left, m_Right, m_Top, m_Bottom;
		};

		Type m_Data[4];
	};
	
	SRect()
		: m_Left(0)
		, m_Right(0)
		, m_Top(0)
		, m_Bottom(0)
	{ }

	SRect(Type a_Left, Type a_Right, Type a_Top, Type a_Bottom)
		: m_Left(a_Left)
		, m_Right(a_Right)
		, m_Top(a_Top)
		, m_Bottom(a_Bottom)
	{ }

	bool Contains(const Math::Vec2& a_Position)
	{
		return (a_Position.x >= m_Left &&
				a_Position.x <= m_Right &&
				a_Position.y >= m_Bottom &&
				a_Position.y <= m_Top);
	}

	template <typename OtherType>
	bool Contains(const SRect<OtherType>& a_Rect)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (Contains(a_Rect.m_Data[i]))
			{
				return true;
			}
		}

		return false;
	}
};

}