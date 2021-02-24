#pragma once

namespace Fade {

template <typename ElementType>
class TList
{
	ElementType* m_Data;
public:
	/**
	 * Default constructor
	 */
	TList() 
		: m_Data(nullptr)
	{ }

	/**
	 * Default destructor
	 */
	~TList()
	{
		
	}

	void Add(const ElementType& a_Element)
	{

	}

};

}