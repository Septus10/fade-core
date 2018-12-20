#pragma once 

#include <Core/Containers/UniquePointer.hpp>
#include <memory>

namespace Fade {

template <class ItemClass>
class TNTreeNode
{
public:
	std::shared_ptr<ItemClass>& GetObject()
	{
		return m_Object;
	}

private:
	std::shared_ptr<ItemClass> m_Element;
	
};

template <class ItemClass>
class TNTree
{
public:
	TNTreeNode<ItemClass>& Find(std::shared_ptr<ItemClass>& a_ObjectToFind)
	{

	}

private:
	std::shared_ptr<TNTreeNode<ItemClass>> m_MainNode;
};

}